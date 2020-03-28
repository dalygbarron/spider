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
                sf::IntRect const rat;
                float const radius;

                /**
                 * Constructor that adds the stuff.
                 * @param rat    is the image to draw such bullets with.
                 * @param radius is the radius of such bullets.
                 */
                Prototype(sf::IntRect rat, float radius);
        };

        Bullet::Prototype const &prototype;
        Actor const &parent;

        /**
         * Creates the bullet by telling it what it's prototype is.
         * @param prototype is the prototype of the bullet.
         * @param parent    is the parent of the bullet who cannot be harmed by
         *                  it.
         */
        Bullet(Bullet::Prototype const &prototype, Actor const &parent);
};

#endif
