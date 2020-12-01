#include "Screen.hh"

EditorScreen::EditorScreen(Core &core): Screen(core) {
    // Nothing else thus far.
}

void EditorScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Game")) {
                printf("newing\n");
            }
            if (ImGui::MenuItem("Load Game")) {
                printf("loading\n");
            }
            if (ImGui::MenuItem("Save Game")) {
                printf("saving\n");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Close All But Current Tab")) {
                printf("closing things\n");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (!ImGui::Begin("Editor")) {
        ImGui::End();
        return;
    }
    if (ImGui::BeginTabBar("tabs")) {
        if (ImGui::BeginTabItem("start.plx")) {
            ImGui::Text("Hnelli");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("cirno.pex")) {
            ImGui::Text("Idiota");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void EditorScreen::draw(sf::RenderTarget &target, int top) const {
    ImGui::SFML::Render(target);
}

int EditorScreen::isTransparent() const {
    return false;
}
