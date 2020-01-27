#include "Screen.hh"
#include "Const.hh"

EntityScreen::EntityScreen(Core &core, Entity &entity):
    Screen(core),
    entity(entity)
{
    // Does nothing right now.
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen *EntityScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::Begin("Entity")) {
        if (ImGui::Button("save")) {
            spdlog::info("Pressed the button hell yeah man");
        }
    }
    ImGui::End();
}

void EntityScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.clear();
    target.draw(this->picture, states);
    ImGui::SFML::Render(target);
}

