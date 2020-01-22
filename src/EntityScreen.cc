#include "Screen.hh"

EntityScreen::EntityScreen(Entity *entity) {
    this->entity = entity;
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen *EntityScreen::update(float delta, sf::Window &window) {
    // TODO: stuff.
}

void EntityScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    // TODO: stuff.
}

