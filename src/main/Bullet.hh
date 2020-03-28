#ifndef BULLET_H
#define BULLET_H

#include "Mob.hh"
#include "Actor.hh"

/**
 * A nice bullet flying through the air.
 */
class Bullet: public Mob {
    public:
        /**
         * Prototypal bullet.
         */
        class Prototype {
            public:
                sf::IntRect rat;

        };

        Bullet::Prototype const &prototype;
        Actor const &parent;

        /**
         * Creates the bullet by telling it what it's prototype is.
         * @param prototype is the prototype of the bullet.
         */
        Bullet(Bullet::Prototype const &prototype, Actor const &parent);
};

#endif
