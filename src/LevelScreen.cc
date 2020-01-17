#include "Screen.hh"
#include "Const.hh"

LevelScreen::LevelScreen(Level *level) {
    this->level = level;
    this->back.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->fileDialog.SetTitle("bastard");
    this->fileDialog.SetTypeFilters({ ".png"});
    // TODO: this shader really ought to be kept and reused by other level
    //       screens since it is always the same stuff but I will do that
    //       later.
    this->shader.setUniform("angle", this->camera);
    this->shader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!this->shader.loadFromMemory(
        Const::SKY_SHADER, sf::Shader::Fragment
    )) {
        fprintf(stderr, "Couldn't start the sky shader.\n");
    }
    Picture const *levelPicture = level->getPicture();
    if (levelPicture) {
        this->back.setTexture(&(levelPicture->getTexture()));
    }
}

LevelScreen::~LevelScreen() {
    delete this->level;
}

Screen *LevelScreen::update(float delta, sf::Window &window) {
    // fuck around with camera.
    camera.x += delta;
    this->shader.setUniform("angle", camera);
    // Now do the gui.
    if(ImGui::Begin(level->file.c_str())) {
        if (ImGui::Button(level->getClean() ? "Save" : "+Save+")) {
            // TODO: stuff
        }
        if (ImGui::Button("Select Pic")) fileDialog.Open();
        ImGui::SameLine();
        Picture const *picture = level->getPicture();
        if (picture) {
            ImGui::Text(
                "Current Picture: %s",
                picture->path.c_str()
            );
        }
        ImGui::Separator();
        ImGui::Text("Entities");
        ImGui::BeginChild("Scrolling", ImVec2(150, 0), true);
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
        ImGui::DragFloat("longitude", &longitude, 0.01f, -Const::PI, Const::PI, "%.2f");
        ImGui::DragFloat("latitude", &latitude, 0.01f, -Const::PI, Const::PI, "%.2f");
        ImGui::Separator();
        ImGui::EndChild();
        ImGui::EndGroup();
        ImGui::End();
    }
    // File selector.
    fileDialog.Display();
    if (fileDialog.HasSelected()) {
        window.setTitle(fileDialog.GetSelected().string());
        fileDialog.ClearSelected();
    }
    return this;
}

void LevelScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    states.shader = &(this->shader);
    target.draw(back, states);
    ImGui::SFML::Render(target);
}

