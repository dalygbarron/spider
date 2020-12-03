#include "FileEditor.hh"

EntityFileEditor::EntityFileEditor(Entity &entity): entity(entity) {}

void EntityFileEditor::update(float delta) {
    ImGui::Begin("Message from management");
    ImGui::Text("I'll code this later");
    ImGui::End();
}

void EntityFileEditor::draw(sf::RenderTarget &target) const {}
