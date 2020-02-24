#include "Screen.hh"
#include "FileIO.hh"
#include "Const.hh"
#include "Util.hh"

LevelScreen::LevelScreen(Core &core, Level &level):
    Screen(core),
    level(level),
    entitySelector(
        ImGuiFileBrowserFlags_EnterNewFilename |
        ImGuiFileBrowserFlags_EnterNewFilename
    )
{
    this->back.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->backgroundSelector.SetTitle("Select level background image");
    this->backgroundSelector.SetTypeFilters({".png"});
    this->entitySelector.SetTitle("Select entity to include");
    this->entitySelector.SetTypeFilters({".xml"});
    // TODO: this shader really ought to be kept and reused by other level
    //       screens since it is always the same stuff but I will do that
    //       later.
    this->shader.setUniform("angle", this->camera);
    this->shader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!this->shader.loadFromMemory(
        Const::SKY_SHADER, sf::Shader::Fragment
    )) {
        spdlog::error("Couldn't start the sky shader");
    }
    this->back.setTexture(&this->level.getPic(), true);
    this->textEditor.SetLanguageDefinition(
        TextEditor::LanguageDefinition::Lua()
    );
    this->textEditor.SetText(this->level.script);
    for (Instance const &instance: this->level.instances) {
        if (instance.entity) this->loadEntity(instance.entity->file.c_str());
    }
}

LevelScreen::~LevelScreen() {
    // Doesn othingg.
}

Instance const *LevelScreen::getInstanceWithName(char const *name) const {
    for (Instance const &instance: this->level.instances) {
        if (instance.name == name) return &instance;
    }
    return NULL;
}

void LevelScreen::removeInstance(int index) {
    // TODO: this.
}

Instance &LevelScreen::addInstance(Entity const *entity) {
    int n = this->level.instances.size();
    this->level.instances.resize(n + 1);
    Instance *instance = this->level.instances.data() + n;
    instance->entity = entity;
    char const *baseName;
    char name[LevelScreen::NAME_BUFFER_SIZE];
    if (entity) {
        baseName = entity->name.c_str();
        instance->pos = this->camera;
    } else {
        for (int i = 0; i < 3; i++) {
            instance->mesh.addVertex(sf::Vector2f(
                cos(i * Const::DOUBLE_PI / 3) / 4 + this->camera.x,
                sin(i * Const::DOUBLE_PI / 3) / 4 + this->camera.y
            ));
        }
        baseName = "shape";
    }
    snprintf(name, LevelScreen::NAME_BUFFER_SIZE, "%s", baseName);
    int iteration = 0;
    while (this->getInstanceWithName(name)) {
        iteration++;
        snprintf(
            name,
            LevelScreen::NAME_BUFFER_SIZE,
            "%s%d",
            baseName,
            iteration
        );
    }
    instance->name = name;
    return *instance;
}

void LevelScreen::update(float delta, sf::RenderWindow &window) {
    this->shader.setUniform("angle", camera);
    // Now do the gui.
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::Begin(this->level.file.c_str())) {
        if (ImGui::Button("Save")) {
            this->level.script = this->textEditor.GetText();
            FileIO::saveLevel(this->level);
        }
        if (ImGui::Button("Select Pic")) this->backgroundSelector.Open();
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::Text("Entities");
        ImGui::SameLine();
        ImGui::Button("-");
        ImGui::SameLine();
        if (ImGui::Button("+")) this->entitySelector.Open();
        ImGui::BeginChild("EntityList", ImVec2(0, 100), true);
        for (int i = 0; i < this->entities.size(); i++) {
            if (ImGui::Selectable(
                this->entities[i].c_str(),
                this->selectedEntity == i
            )) {
                this->selectedEntity = i;
            }
        }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::Text("Instances");
        ImGui::SameLine();
        if (ImGui::Button("--") && this->selectedInstance) {
            this->level.instances.erase(
                this->level.instances.begin() +
                    (this->selectedInstance - this->level.instances.data())
            );
            this->selectedInstance = NULL;
        }
        ImGui::SameLine();
        if (ImGui::Button("+shape")) {
            Instance &instance = this->addInstance(NULL);
        }
        ImGui::SameLine();
        if (ImGui::Button("+entity")) {
            Entity const *entity = this->core.entityRepository.get(
                this->entities[this->selectedEntity].c_str()
            );
            if (!entity) {
                spdlog::error(
                    "Entity for key '{}' is null, program will probably crash",
                    this->entities[this->selectedEntity].c_str()
                );
            }
            Instance &instance = this->addInstance(entity);
        }
        ImGui::BeginChild("InstanceList", ImVec2(150, 0), true);
        for (Instance &instance: this->level.instances) {
            if (ImGui::Selectable(
                instance.name.c_str(),
                &instance == this->selectedInstance
            )) {
                this->selectedInstance = &instance;
                this->selected = 0;
                if (instance.entity) this->camera = instance.pos;
                else this->camera = instance.mesh.getCentre();
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        // TODO: if a shape is selected do the shape menu.
        this->entityMenu();
    }
    ImGui::End();
    // Text editor.
    auto cpos = this->textEditor.GetCursorPosition();
    if (ImGui::Begin("Script Editor")) {
        this->textEditor.Render("brexit");
    }
    ImGui::End();
    // pic selector.
    this->backgroundSelector.Display();
    if (this->backgroundSelector.HasSelected()) {
        ghc::filesystem::path file =
            this->backgroundSelector.GetSelected().string();
        this->level.setPic(ghc::filesystem::relative(file));
        this->back.setTexture(&this->level.getPic(), true);
        this->backgroundSelector.ClearSelected();
    }
    // entity selector
    this->entitySelector.Display();
    if (this->entitySelector.HasSelected()) {
        ghc::filesystem::path file = this->entitySelector.GetSelected().string();
        ghc::filesystem::path relative = ghc::filesystem::relative(file);
        spdlog::info("loading entity at {}", relative.c_str());
        this->loadEntity(relative.c_str());
        this->entitySelector.ClearSelected();
    }
}

void LevelScreen::onClick(sf::Mouse::Button button, sf::Vector2f pos) {
    if (button == sf::Mouse::Button::Left) {
        sf::Vector2f coordinate = Util::screenToSphere(pos, this->camera);
        // Find the closest thing.
        float distance = 0.2;
        this->selectedInstance = NULL;
        for (Instance &instance: this->level.instances) {
            float newDistance = instance.distance(coordinate);
            if (newDistance < distance) {
                distance = newDistance;
                this->selectedInstance = &instance;
                if (!instance.entity) {
                    this->selected = instance.mesh.getClosestVertex(
                        coordinate,
                        0.2
                    );
                }
            }
        }
    }
}

void LevelScreen::onKey(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Space) {
        if (this->selected != -1) {
            this->selectedInstance->mesh.split(this->selected);
            this->selected++;
        }
    } else if (key == sf::Keyboard::Key::Backspace) {
        if (this->selected != -1 &&
            this->selectedInstance->mesh.getVertices().size() > 2
        ) {
            this->selectedInstance->mesh.remove(this->selected);
            this->selected--;
            if (this->selected < 0) {
                this->selected = this->selectedInstance->mesh.getVertices().size() - 1;
            }
        }
    }
}

void LevelScreen::onDrag(sf::Vector2f delta, sf::Vector2f pos) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (this->selectedInstance) {
            if (this->selectedInstance->entity) {
                    sf::Vector2f spherePos = Util::screenToSphere(
                        pos,
                        this->camera
                    );
                    this->selectedInstance->pos.x = spherePos.x;
                    this->selectedInstance->pos.y = spherePos.y;
            } else {
                sf::Vector2f *vertex = this->selectedInstance->mesh.getVertex(
                    this->selected
                );
                if (vertex) {
                    sf::Vector2f spherePos = Util::screenToSphere(
                        pos,
                        this->camera
                    );
                    vertex->x = spherePos.x;
                    vertex->y = spherePos.y;
                }
            }
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        this->camera.x -= delta.x * 0.003;
        this->camera.y -= delta.y * 0.003;
    }
    if (this->camera.y > Const::HALF_PI) this->camera.y = Const::HALF_PI;
    if (this->camera.y < -Const::HALF_PI) this->camera.y = -Const::HALF_PI;
}

void LevelScreen::onScroll(int delta) {
    if (this->selectedInstance) this->selectedInstance->size += delta * 0.1;
}

Entity *LevelScreen::loadEntity(char const *key) {
    int n = this->entities.size();
    this->entities.resize(n + 1);
    this->entities[n] = key;
    Entity *entity = this->core.entityRepository.get(key);
    if (!entity) {
        spdlog::error(
            "Entity for key '{}' is null, program will probably crash",
            key
        );
    }
    return entity;
}

void LevelScreen::entityMenu() {
    ImGui::BeginGroup();
    ImGui::BeginChild("itemView",ImVec2(
        0,
        -ImGui::GetFrameHeightWithSpacing()
    ));
    ImGui::Text("Number: 6");
    float latitude;
    float longitude;
    ImGui::DragFloat(
        "longitude",
        &longitude,
        0.01f,
        -Const::PI,
        Const::PI,
        "%.2f"
    );
    ImGui::DragFloat(
        "latitude",
        &latitude,
        0.01f,
        -Const::HALF_PI,
        Const::HALF_PI,
        "%.2f"
    );
    ImGui::Separator();
    ImGui::EndChild();
    ImGui::EndGroup();
}

void LevelScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.clear();
    states.shader = &(this->shader);
    target.draw(back, states);
    this->core.renderer.batch.clear();
    sf::Vector3f floor = Util::sphereToScreen(
        sf::Vector2f(0, Const::HALF_PI * ((this->camera.y > 0) ? 1 : -1)),
        this->camera
    );
    for (Instance const &instance: this->level.instances) {
        if (instance.entity) {
            sf::Vector3f pos = Util::sphereToScreen(
                instance.pos,
                this->camera
            );
            if (pos.z < 0) continue;
            float angle;
            if (this->camera.y > 0) {
                angle = atan2(floor.x - pos.x, floor.y - pos.y) *
                    sin(this->camera.y);
            } else {
                angle = atan2(pos.x - floor.x, pos.y - floor.y) *
                    sin(this->camera.y);
            }
            this->core.renderer.batch.draw(
                instance.entity->sprite,
                sf::Vector2f(pos.x, pos.y),
                instance.entity->offset,
                angle * ((this->camera.y > 0) ? -1 : 1),
                sf::Vector2f(instance.size, instance.size)
            );
            this->core.renderer.point(
                sf::Vector2f(pos.x, pos.y),
                this->selectedInstance == &instance
            );
        } else {
            this->core.renderer.sphereMesh(
                instance.mesh,
                this->camera,
                (this->selectedInstance == &instance) ? this->selected : -1 
            );
        }
    }
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}

