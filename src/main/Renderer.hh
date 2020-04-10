#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
#include "Rat.hh"
#include "Patch.hh"
#include "Mesh.hh"
#include "Font.hh"
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

        /**
         * A mouse cursor type.
         */
        enum class CursorType {
            Pointer,
            Talk,
            Move,
            Use,
            N
        };

        sf::IntRect pointRat;
        sf::IntRect pointHighlightRat;
        sf::IntRect lineRat;
        sf::IntRect lineHighlightRat;
        sf::IntRect nodeRat;
        sf::IntRect nodeHighlightRat;
        sf::IntRect battleRat;
        sf::IntRect font;
        sf::IntRect battleFont;
        sf::IntRect cursorRats[static_cast<int>(Renderer::CursorType::N)];
        Patch boxPatch;
        Patch boxHighlightPatch;
        Patch panelPatch;
        Patch buttonPatch;
        Patch buttonDepressedPatch;
        mutable RatBatch batch;

        /**
         * Passes in the texture of the spritesheet, and also the rectangles
         * containing certain nice things.
         * @param sprites is the spritesheet texture.
         */
        Renderer(sf::Texture const &sprites);

        /**
         * Sets the cursor rat for a certain type of cursor.
         * @param rat  is the rat to use.
         * @param type is the type of cursor this is for.
         */
        void setCursorRat(sf::IntRect rat, Renderer::CursorType type);

        /**
         * Draw a crosshair over the given point on the screen.
         * @param pos       is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void point(sf::Vector2f pos, int highlight) const;

        /**
         * Draw a round node thingy over the given point on the screen.
         * @param pos is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void node(sf::Vector2f pos, int highlight) const;

        /**
         * Draw a cursor pointing to the given point on the screen.
         * @param pos    is the location the cursor should point to.
         * @param cursor is the cursor type to draw.
         */
        void cursor(sf::Vector2f pos, Renderer::CursorType cursor) const;

        /**
         * Draw a box around the given rectangle on the screen.
         * @param pos is the rectangle to draw around.
         * @param highlight is whether to make the box highlighted.
         */
        void box(sf::FloatRect pos, int highlight) const;

        /**
         * Draw a panel around the given rectangle on the screen.
         * @param pos is the rectangle to draw the panel around.
         */
        void panel(sf::FloatRect pos) const;

        /**
         * Draw a button around the given rectangle on the screen.
         * @param pos       is the rectangle to draw the button around.
         * @param depressed is whether to draw the button as depressed or not.
         */
        void button(sf::FloatRect pos, int depressed) const;

        /**
         * Draw a line between two points.
         * @param start     is where on the screen the line starts.
         * @param end       is where on the screen the line ends.
         * @param highlight is whether to draw the line highlighted.
         */
        void line(sf::Vector2f start, sf::Vector2f, int highlight) const;

        /**
         * Draw a node with a line coming out of it.
         * @param start     is the start where the node is.
         * @param end       is where the line ends.
         * @param highlight is whether to draw it highlighted or normal.
         */
        void club(sf::Vector2f start, sf::Vector2f end, int highlight) const;

        /**
         * Render a sphere mesh rotated in accordance with the given camera
         * orientation.
         * @param mesh      is the mesh to draw.
         * @param camera    is the camera to rotate it in accordance with.
         * @param highlight the index of the node to highlight. If you want
         *                  none to be highlighted give an index that does not
         *                  appear eg -1.
         */
        void sphereMesh(
            Mesh const &mesh,
            sf::Vector2f camera,
            int highlight
        ) const;

        /**
         * Renders an arc from one angle to another.
         * @param pos    is the middle of the arc.
         * @param radius is the radius of the arc.
         * @param a      is the starting angle.
         * @param b      is the ending angle.
         */
        void arc(sf::Vector2f pos, float radius, float a, float b) const;

        /**
         * Writes some text on the screen using the default font.
         * @param content is the text to write.
         * @param pos     is the location to write it at.
         * @param 
         */
        void text(char const *content, sf::Vector2f pos) const;

        /**
         * Writes some text on the screen using a given font.
         * @param content is the text to write.
         * @param pos     is the location to write it at.
         * @param font    is the font to use to write the text.
         */
        void text(
            char const *content,
            sf::Vector2f pos,
            sf::IntRect font
        ) const;

        /**
         * Renders the given rat in the given spot.
         * @param rat is the rat to render.
         * @param pos is the place on the screen to render it.
         */
        void rat(Rat const &rat, sf::Vector2f pos);

        /**
         * Renders the given rat in the given spot with rotation and shit.
         * @param rat      is the rat to render.
         * @param position is the position on the screen to render it.
         * @param rotation is the rotation to give it.
         * @param scale    is the scale to draw it at.
         * @param flip     is whether to flip it around the vertical axis.
         */
        void rat(
            Rat const &rat,
            sf::Vector2f pos,
            float rotation,
            sf::Vector2f scale,
            int flip
        );

    private:

        void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
