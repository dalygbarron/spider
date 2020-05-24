#ifndef LIMITER_H
#define LIMITER_H

#include <functional>

/**
 * Keeps the logical frame rate of some actions constant while being run at
 * a variable frame rate.
 */
class Limiter {
    public:
        /**
         * Creates the limiter by setting the frame rate it should be aiming
         * for, and then action that it should be performing at that rate.
         * @param rate   is the rate at which the action should be performed as
         *               a number per second.
         */
        Limiter(float rate);

        /**
         * Should be called every frame, and will call the given action
         * function enough times at any given time to have been called as many
         * times as it would have if the program was running at a given fixed
         * frame rate.
         * @param delta  is the amount of time that the previous frame took.
         * @param action is the action to take.
         */
        void update(float delta, std::function<void(float)> const &action);

    private:
        float const rate;
        float const interval;
        float time = 0;
        int i = 0;
};

#endif
