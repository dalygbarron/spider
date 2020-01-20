#include "Screen.hh"
#include "Util.hh"
#include "Const.hh"
#include <cmath>

LevelScreen::LevelScreen(Level *level):
    entitySelector(
        ImGuiFileBrowserFlags_EnterNewFilename |
        ImGuiFileBrowserFlags_EnterNewFilename
    )
{
    this->level = level;
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

Screen::Transition LevelScreen::logic(float delta) {
    Screen::Transition transition(NULL, false);
    this->shader.setUniform("angle", camera);
    // Now do the gui.
    if(ImGui::Begin(level->file.c_str())) {
        if (ImGui::Button(level->getClean() ? "Save" : "+Save+")) {
            // TODO: stuff
        }
        if (ImGui::Button("Select Pic")) this->backgroundSelector.Open();
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
            transition.screen = new LevelScreen(Util::levelFromFile(this->level->file));
        }
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
        ghc::filesystem::path pic = this->backgroundSelector.GetSelected().string();
        Picture *levelPicture = Util::pictureFromFile(pic);
        this->level->setPicture(levelPicture);
        this->back.setTexture(&(levelPicture->getTexture()));
        this->backgroundSelector.ClearSelected();
    }
    // entity selector
    this->entitySelector.Display();
    if (this->entitySelector.HasSelected()) {
        ghc::filesystem::path file = this->entitySelector.GetSelected().string();
        Entity *entity = Util::entityFromFile(file);
        // TODO: more stuff.
        this->entitySelector.ClearSelected();
    }
    return transition;
}

void LevelScreen::onClick(sf::Mouse::Button button) {
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
    states.shader = &(this->shader);
    target.draw(back, states);
}

