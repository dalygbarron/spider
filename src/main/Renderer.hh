#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
#include "Rat.hh"
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

        Rectangle pointRat;
        Rectangle pointHighlightRat;
        Rectangle lineRat;
        Rectangle lineHighlightRat;
        Rectangle nodeRat;
        Rectangle nodeHighlightRat;
        Rectangle battleRat;
        Rectangle font;
        Rectangle battleFont;
        Rectangle cursorRats[static_cast<int>(Renderer::CursorType::N)];
        Patch boxPatch;
        Patch boxHighlightPatch;
        Patch panelPatch;
        Patch buttonPatch;
        Patch buttonDepressedPatch;
        glm::ivec2 const size;
        mutable RatBatch batch;

        /**
         * Passes in the texture of the spritesheet, and also the rectangles
         * containing certain nice things.
         * @param sprites is the spritesheet texture.
         * @param size    is the window size.
         */
        Renderer(sf::Texture const &sprites, glm::ivec2 size);

        /**
         * Sets the cursor rat for a certain type of cursor.
         * @param rat  is the rat to use.
         * @param type is the type of cursor this is for.
         */
        void setCursorRat(Rectangle rat, Renderer::CursorType type);

        /**
         * Draw a crosshair over the given point on the screen.
         * @param pos       is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void point(glm::vec2 pos, int highlight) const;

        /**
         * Draw a round node thingy over the given point on the screen.
         * @param pos is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void node(glm::vec2 pos, int highlight) const;

        /**
         * Draw a cursor pointing to the given point on the screen.
         * @param pos    is the location the cursor should point to.
         * @param cursor is the cursor type to draw.
         */
        void cursor(glm::vec2 pos, Renderer::CursorType cursor) const;

        /**
         * Draw a box around the given rectangle on the screen.
         * @param pos is the rectangle to draw around.
         * @param highlight is whether to make the box highlighted.
         */
        void box(Rectangle pos, int highlight) const;

        /**
         * Draw a panel around the given rectangle on the screen.
         * @param pos is the rectangle to draw the panel around.
         */
        void panel(Rectangle pos) const;

        /**
         * Draw a button around the given rectangle on the screen.
         * @param pos       is the rectangle to draw the button around.
         * @param depressed is whether to draw the button as depressed or not.
         */
        void button(Rectangle pos, int depressed) const;

        /**
         * Draw a line between two points.
         * @param start     is where on the screen the line starts.
         * @param end       is where on the screen the line ends.
         * @param highlight is whether to draw the line highlighted.
         */
        void line(glm::vec2 start, glm::vec2, int highlight) const;

        /**
         * Draw a node with a line coming out of it.
         * @param start     is the start where the node is.
         * @param end       is where the line ends.
         * @param highlight is whether to draw it highlighted or normal.
         */
        void club(glm::vec2 start, glm::vec2 end, int highlight) const;

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
            glm::mat4 camera,
            int highlight
        ) const;

        /**
         * Renders an arc from one angle to another.
         * @param pos    is the middle of the arc.
         * @param radius is the radius of the arc.
         * @param a      is the starting angle.
         * @param b      is the ending angle.
         */
        void arc(glm::vec2 pos, float radius, float a, float b) const;

        /**
         * Writes some text on the screen using the default font.
         * @param content is the text to write.
         * @param pos     is the location to write it at.
         * @param 
         */
        void text(char const *content, glm::vec2 pos) const;

        /**
         * Writes some text on the screen using a given font.
         * @param content is the text to write.
         * @param pos     is the location to write it at.
         * @param font    is the font to use to write the text.
         */
        void text(
            char const *content,
            glm::vec2 pos,
            Rectangle font
        ) const;

    private:
        void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
