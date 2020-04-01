#include "Font.hh"

Font::Font():
    bounds(0, 0, 0, 0),
    character(0, 0)
{

}

Font::Font(sf::IntRect bounds):
    bounds(bounds),
    character(bounds.width / 16, bounds.height / 16)
{
    // Nothing else.
}

sf::IntRect Font::get(char c) const {
    return sf::IntRect(
        this->bounds.left + (c % 16) * this->character.x,
        this->bounds.top + (c / 16) * this->character.y,
        this->character.x,
        this->character.y
    );
}
