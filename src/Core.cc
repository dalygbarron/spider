#include "Core.hh"

Core::Core(sf::Font *font):
    font(font),
    renderer(
        spritesheet.getTexture(),
        font->getTexture(20),
        font->getTexture(30)
    )
{
    // Does nothing else.
}

Core::~Core() {
    delete this->font;
}
