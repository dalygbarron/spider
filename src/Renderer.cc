#include "Renderer.hh"

Renderer::Renderer(
    sf::Texture const &sprites,
    sf::Texture const &font,
    sf::Texture const &bigFont
):
    spriteBatch(sprites),
    fontBatch(font),
    bigFontBatch(bigFont)
{
    // does nothing else.
}

void Renderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(this->spriteBatch);
    target.draw(this->fontBatch);
    target.draw(this->bigFontBatch);
}
