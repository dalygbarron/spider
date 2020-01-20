#include "Screen.hh"

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen::Transition EntityScreen::logic(float delta) {
    return Screen::Transition(NULL, false);
}

void EntityScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    // TODO: stuff.
}

