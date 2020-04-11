#ifndef FISH_H
#define FISH_H

#include "Entity.hh"
#include <SFML/Graphics.hpp>

/**
 * Something that can move around in 3d in the adventure screen behind the
 * photosphere set.
 */
class Fish {
    public:
        sf::Vector3f position;
        sf::Vector3f velocity;
        Entity const *entity;
        int alive = true;

        /**
         * Updates the fish's location for 1 frame.
         */
        void update();
};

#endif
