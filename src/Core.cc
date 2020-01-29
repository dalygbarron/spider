#include "Core.hh"

Core::Core(sf::Font *font):
    font(font),
    renderer(this->spritesheet.getTexture())
{
    // Does nothing else.
}

Core::~Core() {
    delete this->font;
}

sf::Font &Core::getFont() {
    return *this->font;
}
