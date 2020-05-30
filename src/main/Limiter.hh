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
         * @param rate is the rate at which the action should be performed as
         *             a number per second.
         */
        Limiter(float rate);

        /**
         * When you want to control the rate at which something happens within
         * a variable timestep, you simply call this function as often as
         * possible with the timestep, and it tells you the number of times you
         * should perform your function in that frame to be up to date.
         * @param delta is the step of the current frame.
         * @return the number of times to run the limited function to be up to
         *         date.
         */
        int update(float delta);

    private:
        float const rate;
        float const interval;
        float time = 0;
        int i = 0;
};

#endif
