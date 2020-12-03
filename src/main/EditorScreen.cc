#include "Screen.hh"

EditorScreen::EditorScreen(Core &core): Screen(core) {
    // Nothing else thus far.
}

void EditorScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
}

void EditorScreen::draw(sf::RenderTarget &target, int top) {
    if (this->active) this->active->draw(target);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                printf("saving\n");
            }
            if (ImGui::MenuItem("Save and Exit")) {
                printf("Savnig and exiting\n");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    this->fileBrowser();
    ImGui::SFML::Render(target);
}

int EditorScreen::isTransparent() const {
    return false;
}

void directoryTree(ghc::filesystem::path path, bool top) {
    if (!ghc::filesystem::exists(path)) return;
    if (top || ImGui::TreeNode(path.filename().c_str())) {
        ghc::filesystem::directory_iterator filesEnd;
        for (ghc::filesystem::directory_iterator it(path);
                it != filesEnd;
                ++it
        ) {
            if (ghc::filesystem::is_directory(it->status())) {
                directoryTree(it->path(), false);
            } else {
                if (ImGui::Button(ghc::filesystem::relative(
                    it->path(),
                    path
                ).c_str())) {
                    printf("Clicked something\n");
                }
            }
        }
        if (!top) ImGui::TreePop();
    }
}

void EditorScreen::fileBrowser() {
    ImGui::Begin("Active Files");
    ImGui::Text("Brexit");
    ImGui::End();
    ImGui::Begin("File Tree");
    ghc::filesystem::path pwd = ghc::filesystem::current_path();
    directoryTree(pwd, true);
    ImGui::End();
}
