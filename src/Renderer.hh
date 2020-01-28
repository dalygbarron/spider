#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
#include <SFML/Graphics.hpp>

/**
 * Renders shit mane.
 */
class Renderer: public sf::Drawable {
    public:
        RatBatch spriteBatch;

        /**
         * Passes in the textures of the three sprite things.
         * @param sprites is the spritesheet texture.
         * @param font    is the font texture.
         * @param bigFont is the big font texture.
         */
        Renderer(
            sf::Texture const &sprites,
            sf::Texture const &font,
            sf::Texture const &bigFont
        );

    private:
        sf::Font *font;
        RatBatch fontBatch;
        RatBatch bigFontBatch;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif
