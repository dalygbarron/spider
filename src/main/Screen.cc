#include "Screen.hh"

Screen::Screen(Core &core): core(core) {
    // Does nothing else.
}

Screen::~Screen() {
    // Does nothing.
}

int Screen::isTransparent() const {
    return false;
}

void Screen::onStart() {
    // Do nothing by default.
}

void Screen::onReveal(int value) {
    // Do nothing by default.
}

void Screen::onClick(sf::Mouse::Button button, glm::ivec2 pos) {
    // Do nothing by default.
}

void Screen::onDrag(glm::ivec2 delta, glm::ivec2 pos) {
    // Do nothing by default.
}

void Screen::onScroll(int delta) {
    // Do nothing by default.
}

void Screen::onKey(sf::Keyboard::Key key) {
    // Do nothing by default
}
