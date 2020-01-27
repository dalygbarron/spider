#ifndef RAT_BATCH_H
#define RAT_BATCH_H

#include <SFML/Graphics.hpp>

class RatBatch: public sf::Drawable {
    public:
        /**
         * Create the rat batch by telling it what texture to draw with.
         * @param texture is the texture that the given sprites will belong to.
         */
        RatBatch(sf::Texture const &texture);

        /**
         * Clear everything off the batch.
         */
        void clear();

        /**
         * Draw the given sprite with it's normal size centred on the given
         * spot.
         * @param sprite is the portion of the batch's texture to draw.
         * @param pos    is the point on the screen to draw it.
         */
        void draw(sf::IntRect sprite, sf::Vector2f pos);

        /**
         * Draw the given sprite with a given scale on the given spot.
         * @param sprite is the sprite to draw.
         * @param pos    is the point for the middle of the sprite to be.
         * @param rot    is the rotation with which to draw the sprite.
         * @param scale  is the scale at which to draw the sprite.
         */
        void draw(
            sf::IntRect sprite,
            sf::Vector2f pos,
            float rot,
            sf::Vector2f scale
        );

        /**
         * Draw the given sprite into the given rectangle on the screen with
         * stretching if necessary.
         * @param sprite is the portion of the batch's texture to draw.
         * @param pos    is the rectangle to fit it in on the screen.
         */
        void draw(sf::IntRect sprite, sf::FloatRect pos);

    private:
        int n;
        sf::Texture const &texture;
        std::vector<sf::Vertex> vertices;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif
