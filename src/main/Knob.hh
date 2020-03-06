#ifndef KNOB_H
#define KNOB_H

#include "SoundPlayer.hh"
#include <SFML/Graphics.hpp>

/**
 * Forward declaration.
 */
class Renderer;

/**
 * Generic superclass of gui knobs like buttons, boxes, sliders, etc.
 */
class Knob {
    public:
        sf::FloatRect const shape;
        int const id;

        /**
         * Sets the shape and the id.
         * @param x  is the position from the left.
         * @param y  is the position from the right.
         * @param w  is the width.
         * @param h  is the height.
         * @param id is the id number of this knob.
         */
        Knob(int x, int y, int w, int h, int id);

        /**
         * Virtual destructor so sub types can destroy.
         */
        virtual ~Knob();

        /**
         * Updates the knob and potentially returns somethnig nice if something
         * happened.
         * @param mouse       is the current position of the mouse.
         * @param soundPlayer is the sound player object so that the knob can
         *                    make sound effects.
         * @return a pointer to a knob which is null if nothing happened, and
         *         contains a knob that has been activated if a knob has been
         *         activated.
         */
        virtual Knob *update(sf::Vector2f mouse, SoundPlayer &soundPlayer);

        /**
         * Draws the knob on the sprite batch.
         * @param target   is the render target to draw on.
         * @param renderer is the renderer and owner of the sprite batch to
         *                 draw to.
         */
        virtual void draw(
            sf::RenderTarget &target,
            Renderer &renderer
        ) const = 0;
};

/**
 * A type of knob that stores a list of other knobs nicely on a panel.
 */
class PanelKnob: public Knob {
    public:
        int const parts;

        /**
         * Creates the panel and sets how many parts per row it has and it's
         * dimensions.
         * @param x     is the position from the left.
         * @param y     is the position from the right.
         * @param w     is the width.
         * @param h     is the height.
         * @param parts is the number of things per row it will have.
         */
        PanelKnob(int x, int y, int w, int h, int parts);

        /**
         * Deletes the children.
         */
        ~PanelKnob();

        /**
         * Adds a child to the panel. Don't pass null pointers to this for
         * god's sake.
         * @param child is the knob to add as a child.
         */
        void addChild(Knob *child);

        virtual Knob *update(
            sf::Vector2f mouse,
            SoundPlayer &soundPlayer
        ) override;

        virtual void draw(
            sf::RenderTarget &target,
            Renderer &renderer
        ) const override;

    private:
        std::vector<Knob *> children;
};

/**
 * A nice button that you can click on.
 */
class ButtonKnob: public Knob {
    public:
        int enabled = true;

        /**
         * Creates a button knob with an arbitrary knob as it's content.
         * @param x     is the left position.
         * @param y     is the top position.
         * @param w     is the width.
         * @param h     is the height.
         * @param id    is the id number.
         * @param child is the contents of the button.
         */
        ButtonKnob(int x, int y, int w, int h, int id, Knob *child);

        /**
         * Deletes the child.
         */
        ~ButtonKnob();

        virtual Knob *update(
            sf::Vector2f mouse,
            SoundPlayer &soundPlayer
        ) override;

        virtual void draw(
            sf::RenderTarget &target,
            Renderer &renderer
        ) const override;

    private:
        Knob *child;
        int depressed = false;
};

/**
 * Displays some text and wraps it to fit where you put it.
 */
class TextKnob: public Knob {
    public:
        /**
         * Gives the text knob tghe text that it can write.
         * @param text is the text to write. The knob copies this data and then
         *             discards it so if you allocated it dynamically, you must
         *             delete it yourself.
         */
        TextKnob(int x, int y, int w, int h, char const *text);

        virtual void draw(
            sf::RenderTarget &target,
            Renderer &renderer
        ) const override;
    
    private:
        std::string const text;
};

/**
 * Displays a rat scaled to fit in the box you give it without stretching.
 */
class FrameKnob: public Knob {
    public:
        /**
         * Creates teh frameknob and enters the rat.
         * @param x   is the distance from left.
         * @param y   is the distance from top.
         * @param w   is the width.
         * @param y   is the height.
         * @param rat is the rat to draw.
         */
        FrameKnob(sf::IntRect rat);

        virtual void draw(
            sf::RenderTarget &target,
            Renderer &renderer
        ) const override;

    private:
        sf::IntRect rat;
};

#endif
