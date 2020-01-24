#include "Screen.hh"

Screen::Screen(Core &core): core(core) {
    // Does nothing else.
}

Screen::~Screen() {
    // Does nothing.
}

void Screen::onClick(sf::Mouse::Button button) {
    // Do nothing by default.
}

void Screen::onDrag(sf::Mouse::Button button, sf::Vector2f delta) {
    // Do nothing by default.
}
