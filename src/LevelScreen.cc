#include "Screen.hh"
#include "FileIO.hh"
#include "Const.hh"

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
        ImGui::Button("+shape");
        ImGui::SameLine();
        ImGui::Button("+entity");
        ImGui::BeginChild("InstanceList", ImVec2(150, 0), true);
        static int selected = -1;
        static float longitude = 0;
        static float latitude = 0;
        for (int n = 0; n < 50; n++) {
            if (ImGui::Selectable("Bongonr", selected == n)) selected = n;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::BeginChild("itemView",ImVec2(
            0,
            -ImGui::GetFrameHeightWithSpacing()
        ));
        ImGui::Text("Number: %d", selected);
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

void LevelScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.clear();
    states.shader = &(this->shader);
    target.draw(back, states);
    ImGui::SFML::Render(target);
}

