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
                float radius;
        };

        Bullet::Prototype const *prototype;
        unsigned int parent;

        /**
         * Creates the bullet by telling it what it's prototype is. For the
         * love of god don't pass invalid pointers mate.
         * @param prototype is the prototype of the bullet.
         * @param parent    is the parent of the bullet who cannot be harmed by
         *                  it.
         */
        Bullet(Bullet::Prototype const *prototype, unsigned int parent);
};

#endif
