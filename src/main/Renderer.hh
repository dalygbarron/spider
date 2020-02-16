#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
#include "Patch.hh"
#include "Mesh.hh"
#include "Util.hh"
#include <SFML/Graphics.hpp>

/**
 * Renders stuff procedurally. Does not render text because text is a GUI
 * element which is built in a setting up mode anyway so not really required
 * imo.
 */
class Renderer: public sf::Drawable {
    public:
        static int const SPHERE_INTERPOLATION = 5;

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
         * Sets the rat to use to draw lines.
         * @param rat is the rat.
         */
        void setLineRat(sf::IntRect rat);

        /**
         * Set the rat to use to draw highlighted lines.
         * @param rat is the rat to use.
         */
        void setLineHighlightRat(sf::IntRect rat);

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
         * Sets the 9 patch to use to draw boxes.
         * @param patch is the patch to use.
         */
        void setBoxPatch(Patch patch);

        /**
         * Sets the 9 patch to use to draw highlighted boxes.
         * @param patch is the rat to use.
         */
        void setBoxHighlightPatch(Patch patch);

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

        /**
         * Draw a line between two points.
         * @param start     is where on the screen the line starts.
         * @param end       is where on the screen the line ends.
         * @param highlight is whether to draw the line highlighted.
         */
        void line(sf::Vector2f start, sf::Vector2f, int highlight);

        /**
         * Draw a node with a line coming out of it.
         * @param start     is the start where the node is.
         * @param end       is where the line ends.
         * @param highlight is whether to draw it highlighted or normal.
         */
        void club(sf::Vector2f start, sf::Vector2f end, int highlight);

        /**
         * Render a sphere mesh rotated in accordance with the given camera
         * orientation.
         * @param mesh      is the mesh to draw.
         * @param camera    is the camera to rotate it in accordance with.
         * @param highlight the index of the node to highlight. If you want
         *                  none to be highlighted give an index that does not
         *                  appear eg -1.
         */
        void sphereMesh(Mesh const &mesh, sf::Vector2f camera, int highlight);

    private:
        sf::IntRect pointRat;
        sf::IntRect lineRat;
        sf::IntRect lineHighlightRat;
        sf::IntRect nodeRat;
        sf::IntRect nodeHighlightRat;
        Patch boxPatch;
        Patch boxHighlightPatch;

        void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
