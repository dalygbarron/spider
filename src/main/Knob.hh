#ifndef KNOB_H
#define KNOB_H

#include "SoundPlayer.hh"
#include "Renderer.hh"
#include <SFML/Graphics.hpp>

/**
 * Generic superclass of gui knobs like buttons, boxes, sliders, etc.
 */
class Knob {
    public:
        int id = -1;

        /**
         * Virtual destructor so sub types can destroy.
         */
        virtual ~Knob();

        /**
         * Tells you if the knob is baked.
         */
        int isBaked() const;

        /**
         * Gives you the shape of the knob.
         * @return the shape of it's outline.
         */
        sf::FloatRect getShape() const;

        /**
         * Set the shape of the knob and if it has children, them too.
         * @param shape is the size to fit the knob into.
         */
        virtual void bake(sf::FloatRect shape);

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
         * @param renderer is the renderer and owner of the sprite batch to
         *                 draw to.
         */
        virtual void draw(Renderer &renderer) const = 0;

    private:
        int baked = false;
        sf::FloatRect shape;
};

/**
 * A type of knob that stores a list of other knobs nicely on a panel.
 */
class PanelKnob: public Knob {
    public:
        // TODO: this kinda sucks but it's dependent on the sprite patch.
        static int const BORDER = 8;
        int const parts;

        /**
         * Creates the panel and sets how many parts per row it has.
         * @param parts is the number of things per row it will have.
         */
        PanelKnob(int parts);

        /**
         * Adds a child to the panel. Don't pass null pointers to this for
         * god's sake.
         * @param child is the knob to add as a child.
         */
        void addChild(Knob *child);

        virtual void bake(sf::FloatRect shape) override;

        virtual Knob *update(
            sf::Vector2f mouse,
            SoundPlayer &soundPlayer
        ) override;

        virtual void draw(Renderer &renderer) const override;

    private:
        std::vector<Knob *> children;
};

/**
 * A nice button that you can click on.
 */
class ButtonKnob: public Knob {
    public:
        /**
         * Creates a button knob with an arbitrary knob as it's content.
         * @param child is the contents of the button.
         */
        ButtonKnob(Knob *child);

        virtual void bake(sf::FloatRect shape) override;

        virtual Knob *update(
            sf::Vector2f mouse,
            SoundPlayer &soundPlayer
        ) const override;

        virtual void draw(Renderer &renderer) const override;

    private:
        Knob *child;
};

/**
 * Displays some text and wraps it to fit where you put it.
 */
class TextKnob: public Knob {
    public:
        virtual void draw(Renderer &renderer) const override;
};

/**
 * Displays a rat scaled to fit in the box you give it without stretching.
 */
class FrameKnob: public Knob {
    public:
        FrameKnob(sf::IntRect rat);

        virtual void bake(sf::FloatRect shape) override;

        virtual void draw(Renderer &renderer) const override;

    private:
        sf::IntRect rat;
}

#endif
