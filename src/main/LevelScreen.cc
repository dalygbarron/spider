#include "Screen.hh"
#include "FileIO.hh"
#include "Const.hh"
#include "Shaders.hh"
#include "Util.hh"

LevelScreen::LevelScreen(Core &core, Level &level):
    Screen(core),
    level(level),
    camera(0),
    background(Rectangle(glm::vec2(), core.size)),
    entitySelector(
        ImGuiFileBrowserFlags_EnterNewFilename |
        ImGuiFileBrowserFlags_EnterNewFilename
    )
{
    this->background.initFromString(Shaders::SKY_SHADER);
    this->background.setTexture(&level.getPic());
    this->background.setUniform("fov", this->core.fov);
    this->backgroundSelector.SetTitle("Select level background image");
    this->backgroundSelector.SetTypeFilters({".png"});
    this->entitySelector.SetTitle("Select entity to include");
    this->entitySelector.SetTypeFilters({".xml"});
    // TODO: this shader really ought to be kept and reused by other level
    //       screens since it is always the same stuff but I will do that
    //       later.
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
            instance->mesh.addVertex(glm::vec2(
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
    // Update the backgorund.
    glm::mat4 cameraMatrix = Util::camera(this->camera);
    this->background.setUniform("camera", glm::inverse(cameraMatrix));
    this->background.update();
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

void LevelScreen::onClick(sf::Mouse::Button button, glm::ivec2 pos) {
    if (button == sf::Mouse::Button::Left) {
        glm::vec2 coordinate = glm::vec2();
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

void LevelScreen::onDrag(glm::ivec2 prev, glm::ivec2 pos) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (this->selectedInstance) {
            if (this->selectedInstance->entity) {
                    sf::Vector2f spherePos = sf::Vector2f(0, 0);
                    this->selectedInstance->pos.x = spherePos.x;
                    this->selectedInstance->pos.y = spherePos.y;
            } else {
                glm::vec2 *vertex = this->selectedInstance->mesh.getVertex(
                    this->selected
                );
                if (vertex) {
                    glm::vec2 spherePos;
                    vertex->x = spherePos.x;
                    vertex->y = spherePos.y;
                }
            }
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        this->camera += (glm::vec2)(pos - prev) * 0.005f;
    }
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

void LevelScreen::draw(sf::RenderTarget &target, int top) const {
    this->core.renderer.batch.clear();
    this->background.draw(target);
    glm::mat4 cameraMatrix = this->core.getProjection() *
        Util::camera(this->camera);
    sf::Vector3f floor = sf::Vector3f();
    for (Instance const &instance: this->level.instances) {
        if (instance.entity) {
            glm::vec3 cartesian = Util::sphericalToCartesian(instance.pos);
            this->core.renderer.batch.draw(
                instance.entity->sprite,
                cartesian,
                instance.entity->offset,
                0,
                glm::vec2(1),
                cameraMatrix
            );
            this->core.renderer.batch.draw(
                this->core.renderer.pointRat,
                cartesian,
                glm::vec2(0),
                0,
                glm::vec2(1),
                cameraMatrix
            );
        } else {
            this->core.renderer.sphereMesh(
                instance.mesh,
                cameraMatrix,
                (this->selectedInstance == &instance) ? this->selected : -1 
            );
        }
    }
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}

