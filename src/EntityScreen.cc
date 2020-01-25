#include "Screen.hh"
#include "Const.hh"

EntityScreen::EntityScreen(Core &core, Entity &entity):
    Screen(core),
    entity(entity)
{
    this->picture.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->picture.setFillColor(sf::Color(123, 231, 31, 255));
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

