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
    this->texture.setSmooth(true);
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
}

LevelScreen::~LevelScreen() {
    // Doesn othingg.
}

void LevelScreen::removeInstance(int index) {
    // TODO: this.
}

Instance &LevelScreen::addInstance(Entity *entity) {
    int n = this->instances.size();
    this->instances.resize(n + 1);
    Instance *instance = this->instances.data() + n;
    instance->entity = entity;
    return *instance;
}

Screen *LevelScreen::update(float delta, sf::RenderWindow &window) {
    this->shader.setUniform("angle", camera);
    // Now do the gui.
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::Begin(this->level.file.c_str())) {
        if (ImGui::Button(this->level.getClean() ? "Save" : "+Save+")) {
            // TODO: stuff
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
        for (int n = 0; n < 50; n++) {
            if (ImGui::Selectable("entities/mrBungle.xml", n == 3));
        }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::Text("Instances");
        ImGui::SameLine();
        ImGui::Button("-");
        ImGui::SameLine();
        if (ImGui::Button("+shape")) {
            Instance &instance = this->addInstance(NULL);
            instance.mesh.addVertex(sf::Vector2f(0, 0));
        }
        ImGui::SameLine();
        if (ImGui::Button("+entity")) this->addInstance(NULL);
        ImGui::BeginChild("InstanceList", ImVec2(150, 0), true);
        static int selected = -1;
        for (int n = 0; n < 50; n++) {
            if (ImGui::Selectable("Bongonr", selected == n)) selected = n;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        // TODO: if a shape is selected do the shape menu.
        this->entityMenu();
        ImGui::End();
    }
    // pic selector.
    this->backgroundSelector.Display();
    if (this->backgroundSelector.HasSelected()) {
        this->level.pic = this->backgroundSelector.GetSelected().string();
        this->texture.loadFromFile(this->backgroundSelector.GetSelected().string());
        this->back.setTexture(&this->texture, true);
        this->backgroundSelector.ClearSelected();
    }
    // entity selector
    this->entitySelector.Display();
    if (this->entitySelector.HasSelected()) {
        ghc::filesystem::path file = this->entitySelector.GetSelected().string();
        Entity *entity = FileIO::entityFromFile(file);
        // TODO: more stuff.
        this->entitySelector.ClearSelected();
    }
    return this;
}

void LevelScreen::onClick(sf::Mouse::Button button, sf::Vector2f pos) {
}

void LevelScreen::onDrag(sf::Mouse::Button button, sf::Vector2f delta) {
    if (button == sf::Mouse::Button::Right) {
        this->camera.x -= delta.x * 0.003;
        this->camera.y -= delta.y * 0.003;
    }
    if (this->camera.y > Const::HALF_PI) this->camera.y = Const::HALF_PI;
    if (this->camera.y < -Const::HALF_PI) this->camera.y = -Const::HALF_PI;
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
    for (Instance const &instance: this->instances) {
        if (instance.entity) {
            // TODO: draw entity picture with transformed location.
        } else {
            std::vector<sf::Vector2f> const &vertices = instance.mesh.getVertices();
            int n = vertices.size();
            for (int i = 1; i < n; i++) {
                this->core.renderer.club(
                    Util::sphereToScreen(vertices[i - 1], this->camera),
                    Util::sphereToScreen(vertices[i], this->camera),
                    false
                );
            }
            if (n > 0) {
                this->core.renderer.club(
                    Util::sphereToScreen(vertices[n - 1], this->camera),
                    Util::sphereToScreen(vertices[0], this->camera),
                    false
                );
            }
        }
    }
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}

