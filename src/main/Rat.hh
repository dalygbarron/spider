#ifndef RAT_H
#define RAT_H

/**
 * takes a single rat from a rat pack, and allows you to treat that one rat as
 * a spritesheet.
 */
class Rat {
    public:
        /**
         * Represents a nice little animation that the frames of this rat can
         * make.
         */
        class Animation {
            public:
                vector<int> frames;
                unsigned int frameTime;
                int loop;
        };

        /**
         * Creates the rat with the sprite pic and the number of sub frames in
         * each dimension.
         * @param sprite     is the full spritesheet pic.
         * @param dimensions is the number of sub sprites in each dimension.
         */
        Rat(sf::IntRect sprite, sf::Vector2u dimensions);

        /**
         * Moves the rat forward in time one frame's worth. this should be done
         * once per frame.
         */
        void update();

        /**
         * Gives you the right frame to show for the current time and the rat's
         * state.
         * @return the rectangle of texture to show on the screen.
         */
        sf::IntRect getFrame() const;

        /**
         * Adds an animation to the rat, and validates you are not trying to
         * animate indices that are not there since this is used by game
         * developers.
         * @param name      is the name is the name to give the animation.
         * @param animation is the animation that shall go by that name.
         */
        void addAnimation(char const *name, Rat::Animation animation);

        /**
         * Make it start playing the given animation.
         * @param name is the name of the animation to start playing.
         */
        void play(char const *name);

        /**
         * Makes it stop playing the given animation and just stay frozen as it
         * is.
         */
        void stop();

        /**
         * Tells you if an animation is playing.
         * @return true iff it is.
         */
        int isPlaying() const;

    private:
        unsigned int timer = 0;
        int rolling = false;
        Rat::Animation const *current = NULL;
        sf::IntRect rat;
        sf::Vector2u dimensions;
        unordered_map<std::string, Rat::Animation> animations;
};

#endif
