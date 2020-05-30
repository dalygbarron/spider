#ifndef RAT_H
#define RAT_H

#include "Rectangle.hh"
#include "glm/vec2.hpp"
#include <vector>
#include <climits>

/**
 * takes a single rat from a rat pack, and allows you to treat that one rat as
 * a spritesheet.
 */
class Rat {
    public:
        static unsigned int const DEFAULT_SPEED = 11;

        /**
         * Represents a nice little animation that the frames of this rat can
         * make.
         */
        class Animation {
            public:
                std::vector<int> frames;
                unsigned int frameTime = Rat::DEFAULT_SPEED;
                bool loop = true;
        };

        /**
         * Creates the rat.
         * @param rat        is the full spritesheet.
         * @param dimensions the number of sprites along each axis.
         */
        Rat(Rectangle rat, glm::uvec2 dimensions);

        /**
         * Moves the rat forward in time one frame's worth. this should be done
         * once per frame.
         */
        void update();

        /**
         * Gives you the size of a single item from this.
         * @return the size in each dimension.
         */
        glm::uvec2 getSize() const;

        /**
         * Gives you the right frame to show for the current time and the rat's
         * state.
         * @return the rectangle of texture to show on the screen.
         */
        Rectangle getFrame() const;

        /**
         * Gives you a pointer to the currently playing animation if there is
         * one.
         * @return a pointer to the animation or null.
         */
        Rat::Animation const *getCurrent() const;

        /**
         * Make it start playing the given animation.
         * @param animation is the animation to play. Don't pass null pointers.
         * @param priority  is how important this animation is and if the
         *                  currently playing one is more important this one
         *                  will not play.
         */
        void play(Rat::Animation const *animation, int priority);

        /**
         * Makes it stop playing the given animation and just stay frozen as it
         * is.
         */
        void stop();

        /**
         * Tells you if an animation is playing.
         * @return true iff it is.
         */
        bool isPlaying() const;

    private:
        unsigned int timer = 0;
        int rolling = true;
        int priority = INT_MIN;
        Rat::Animation const *current = NULL;
        Rectangle rat;
        glm::uvec2 dimensions;
};

#endif
