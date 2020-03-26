#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

/**
 * A nice bullet flying through the air.
 */
class Bullet {
    public:
        int type;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f gravity;

        /**
         * Creates a blank bullet.
         */
        Bullet();

        /**
         * Moves the bullet.
         * @param delta is the amount of time to simulate passing.
         */
        void update(float delta);
};

#endif
