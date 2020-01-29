#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
#include <SFML/Graphics.hpp>

/**
 * Renders stuff procedurally. Does not render text because text is a GUI
 * element which is built in a setting up mode anyway so not really required
 * imo.
 */
class Renderer: public sf::Drawable {
    public:
        RatBatch batch;

        /**
         * Passes in the texture of the spritesheet, and also the rectangles
         * containing certain nice things.
         * @param sprites is the spritesheet texture.
         */
        Renderer(sf::Texture const &sprites);

        /**
         * Sets the rat to use to draw points.
         * @param rat is the rat to use.
         */
        void setPointRat(sf::IntRect rat);

        /**
         * Sets the rat to use to draw nodes.
         * @param rat is the rat to use.
         */
        void setNodeRat(sf::IntRect rat);

        /**
         * Sets the rat to use to draw highlighted nodes.
         * @param rat is the rat to use.
         */
        void setNodeHighlightRat(sf::IntRect rat);

        /**
         * Sets the rat to use to draw boxes.
         * @param rat is the rat to use.
         */
        void setBoxRat(sf::IntRect rat);

        /**
         * Sets the rat to use to draw highlighted boxes.
         * @param rat is the rat to use.
         */
        void setBoxHighlightRat(sf::IntRect rat);

        /**
         * Draw a crosshair over the given point on the screen.
         * @param pos       is the position to put it.
         */
        void point(sf::Vector2f pos);

        /**
         * Draw a round node thingy over the given point on the screen.
         * @param pos is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void node(sf::Vector2f pos, int highlight);

        /**
         * Draw a box around the given rectangle on the screen.
         * @param pos is the rectangle to draw around.
         * @param highlight is whether to make the box highlighted.
         */
        void box(sf::FloatRect pos, int highlight);

    private:
        sf::IntRect pointRat;
        sf::IntRect nodeRat;
        sf::IntRect nodeHighlightRat;
        sf::IntRect boxRat;
        sf::IntRect boxHighlightRat;

        void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
