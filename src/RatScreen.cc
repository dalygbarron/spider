#include "Screen.hh"

RatScreen::RatScreen(Core &core): Screen(core) {
    // Does nothing else.
}

Screen *RatScreen::update(float delta, sf::Window &window) {
    // TODO: move some pics around.
}

void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // TODO: draw the pics using the to be created sprite batcher.
}
