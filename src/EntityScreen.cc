#include "Screen.hh"

EntityScreen::EntityScreen(Core &core, Entity &entity):
    screen(core),
    entity(entity)
{
    // Does nothing else I think.
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen *EntityScreen::update(float delta, sf::Window &window) {
}

void EntityScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.draw(this->picture, states);
}

