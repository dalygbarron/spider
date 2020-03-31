#ifndef RENDERER_H
#define RENDERER_H

#include "RatBatch.hh"
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
            pointer,
            talk,
            move,
            use,
            n
        };

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
         * Sets the rat to use to draw highlighted points.
         * @param rat is the rat to use.
         */
        void setPointHighlightRat(sf::IntRect rat);

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
         * Sets the rat to use to draw arcs.
         * @param rat is the rat to use.
         */
        void setArcRat(sf::IntRect rat);

        /**
         * Sets the rat for drawing a given type of cursor for.
         * @param rat    is the rat to draw with.
         * @param cursor is the cursor type to draw with that rat.
         */
        void setCursorRat(sf::IntRect rat, CursorType cursor);

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
         * Sets the patch for drawing panels.
         * @param patch is the patch to draw panels with.
         */
        void setPanelPatch(Patch patch);

        /**
         * Sets the patch for drawing buttons.
         * @param patch is the patch to draw buttons with.
         */
        void setButtonPatch(Patch patch);

        /**
         * Sets the patch for drawing depressed buttons for.
         * @param patch is the patch to draw depressed buttons with.
         */
        void setButtonDepressedPatch(Patch patch);

        /**
         * Gives you some measurements that are useful in non rendering
         * contexts to that stuff lines up with what is rendered.
         * @return a constant reference to the measurements which are kept up
         *         to date.
         */
        Measurements const &getMeasurements();

        /**
         * Sets the font to use for normal text drawing.
         * @param font is the font to use from now on.
         */
        void setFont(Font font);

        /**
         * Draw a crosshair over the given point on the screen.
         * @param pos       is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void point(sf::Vector2f pos, int highlight);

        /**
         * Draw a round node thingy over the given point on the screen.
         * @param pos is the position to put it.
         * @param highlight is whether to make the point highlighted.
         */
        void node(sf::Vector2f pos, int highlight);

        /**
         * Draw a cursor pointing to the given point on the screen.
         * @param pos    is the location the cursor should point to.
         * @param cursor is the cursor type to draw.
         */
        void cursor(sf::Vector2f pos, Renderer::CursorType cursor);

        /**
         * Draw a box around the given rectangle on the screen.
         * @param pos is the rectangle to draw around.
         * @param highlight is whether to make the box highlighted.
         */
        void box(sf::FloatRect pos, int highlight);

        /**
         * Draw a panel around the given rectangle on the screen.
         * @param pos is the rectangle to draw the panel around.
         */
        void panel(sf::FloatRect pos);

        /**
         * Draw a button around the given rectangle on the screen.
         * @param pos       is the rectangle to draw the button around.
         * @param depressed is whether to draw the button as depressed or not.
         */
        void button(sf::FloatRect pos, int depressed);

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

        /**
         * Renders an arc from one angle to another.
         * @param pos    is the middle of the arc.
         * @param radius is the radius of the arc.
         * @param a      is the starting angle.
         * @param b      is the ending angle.
         */
        void arc(sf::Vector2f pos, float radius, float a, float b);

        /**
         * Writes some text on the screen using the font.
         * @param content is the text to write.
         * @param pos     is the location to write it at.
         */
        void text(std::string const &content, sf::Vector2f pos);

    private:
        sf::IntRect pointRat;
        sf::IntRect pointHighlightRat;
        sf::IntRect lineRat;
        sf::IntRect lineHighlightRat;
        sf::IntRect nodeRat;
        sf::IntRect nodeHighlightRat;
        sf::IntRect arcRat;
        sf::IntRect cursorRats[static_cast<int>(Renderer::CursorType::n)];
        Patch boxPatch;
        Patch boxHighlightPatch;
        Patch panelPatch;
        Patch buttonPatch;
        Patch buttonDepressedPatch;
        Font font;
        Measurements measurements;

        void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
