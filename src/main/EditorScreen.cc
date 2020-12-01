#include "Screen.hh"

EditorScreen::EditorScreen(Core &core): Screen(core) {
    // Nothing else thus far.
}

void EditorScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Load Game");
            ImGui::MenuItem("Save Game");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tabs")) {
            ImGui::MenuItem("Close All But Current");
            ImGui::MenuItem("Open File Browser");
            ImGui::MenuItem("Open Game Settings");
            ImGui::EndMenu();
        }
        ImGui::Text("Hnelli");
    }
}

void EditorScreen::draw(sf::RenderTarget &target, int top) const {
    ImGui::SFML::Render(target);
}

int EditorScreen::isTransparent() const {
    return false;
}
