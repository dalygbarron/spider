#ifndef MOB_H
#define MOB_H

#include <SFML/Graphics.hpp>

/**
 * Something that moves around in 2D and has a radius like a coicle.
 */
class Mob {
    public:
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f gravity;
        float radius;

        /**
         * Sets the mob's radius. Yeah I know this kinda fucks the flyweight
         * pattern but otherwise I have to lose the collide function or make
         * getRadius an abstract function or something else inefficient or
         * stupid. The question ought to be "will I suffer for lack of these
         * 4 bytes per mob?" and the answer is no so I don't give a fuck.
         * @param radius is the radius to give to the mob.
         */
        Mob(float radius);

        /**
         * moves the mob based on the passage of time.
         */
        virtual void update();

        /**
         * Checks if two mobs are colliding. First it does a manhattan test to
         * disqualify ones that are way off which the majority will be, and
         * then it does a proper distance test.
         * @param a is the first mob.
         * @param b is the second mob.
         * @return true if they are colliding.
         */
        static int collide(Mob const &a, Mob const &b);
};

#endif
