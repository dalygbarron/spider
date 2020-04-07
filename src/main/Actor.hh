#ifndef ACTOR_H
#define ACTOR_H

#include "Mob.hh"
#include "Rat.hh"

/**
 * A character that can move around in a bullet screen and get hit by bullets
 * and stuff.
 */
class Actor: public Mob {
    public:
        Rat rat;
        int dainty = false;
        int maxHp = 0;
        int hp = 0;

        Actor(Rat rat, float radius);

        virtual void update();
};

#endif
