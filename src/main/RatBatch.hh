#ifndef RAT_BATCH_H
#define RAT_BATCH_H

#include "Patch.hh"
#include <SFML/Graphics.hpp>

/**
 * Batches draw calls into one big call where they are all using portions of
 * the same texture, also has to implement various primitive ways of arranging
 * vertices.
 * TODO: if we are not going to implement changing the colour of a sprite drawn
 *       then there is no reason to add code setting vertices to white
 *       everywhere and the code would be a lot less massive without it.
 *       Actually, if I ever want drawing with a tint, maybe I should add
 *       a setTint function to this which then affects drawing after.
 */
class RatBatch: public sf::Drawable {
    public:
        static int const CIRCLE_SEGMENTS = 10;

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
         * @param offset is the amount to offset origin from centre.
         * @param rot    is the rotation with which to draw the sprite.
         * @param scale  is the scale at which to draw the sprite.
         */
        void draw(
            sf::IntRect sprite,
            sf::Vector2f pos,
            sf::Vector2f offset,
            float rot,
            sf::Vector2f scale
        );

        /**
         * Draw a patch somewhere.
         * @param patch is the patch to darw.
         * @param pos   is where to draw it.
         */
        void draw(Patch const &patch, sf::FloatRect pos);

        /**
         * Draw the given sprite into the given rectangle on the screen with
         * stretching if necessary.
         * @param sprite is the portion of the batch's texture to draw.
         * @param pos    is the rectangle to fit it in on the screen.
         */
        void draw(sf::IntRect sprite, sf::FloatRect pos);

        /**
         * Draw the given sprite stretched as a line from one point to another
         * point.
         * @param sprite is the sprite to draw.
         * @param start  is the start to start drawing from.
         * @param end    is the end to stop drawing at.
         */
        void draw(sf::IntRect sprite, sf::Vector2f start, sf::Vector2f end);

        /**
         * Draws an arc from one angle to the other consisting of a hard coded
         * number of diamond shaped segments.
         * @param sprite is the sprite to draw on each segment where the top
         *               and right of the sprite face the outside edges.
         * @param pos    is where the centre of the circle will be on the
         *               screen.
         * @param radius is the radius for the circle to have.
         * @param a      is the starting angle.
         * @param b      is the ending angle.
         */
        void draw(
            sf::IntRect sprite,
            sf::Vector2f pos,
            float radius,
            float a,
            float b
        );

    private:
        int n;
        mutable int clean;
        sf::Texture const &texture;
        std::vector<sf::Vertex> vertices;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif
