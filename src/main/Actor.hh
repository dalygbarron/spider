#ifndef ACTOR_H
#define ACTOR_H

#include "Mob.hh"

/**
 * A character that can move around in a bullet screen and get hit by bullets
 * and stuff.
 */
class Actor: public Mob {
    public:
        sf::IntRect rat;
        int dainty = false;
        int maxHp = 0;
        int hp = 0;

        Actor(sf::IntRect rat, float radius);
};

#endif
