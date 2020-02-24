#include "Screen.hh"

Screen::Screen(Core &core): core(core) {
    // Does nothing else.
}

Screen::~Screen() {
    // Does nothing.
}

void Screen::onClick(sf::Mouse::Button button, sf::Vector2f pos) {
    // Do nothing by default.
}

void Screen::onDrag(sf::Vector2f delta, sf::Vector2f pos) {
    // Do nothing by default.
}

void Screen::onScroll(int delta) {
    // Do nothing by default.
}

void Screen::onKey(sf::Keyboard::Key key) {
    // Do nothing by default
}
