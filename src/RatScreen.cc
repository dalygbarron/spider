#include "Screen.hh"

RatScreen::RatScreen(Core &core): Screen(core) {
    // Does nothing else.
}

Screen *RatScreen::update(float delta, sf::Window &window) {
    // TODO: move some pics around.
    return NULL;
}

void RatScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    this->core.batch.clear();
    for (int i = 0; i < 100; i++) {
        this->core.batch.draw(
            sf::IntRect(i, i, i, i),
            sf::FloatRect(i * 2, i * 3, i, i)
        );
    }
    target.draw(this->core.batch);
    // TODO: draw the pics using the to be created sprite batcher.
}
