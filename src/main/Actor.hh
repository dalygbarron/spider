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
        int dainty = false;
        int maxHp = 0;
        int hp = 0;
        Rat::Animation const *walkAnimation = NULL;
        Rat rat;

        /**
         * Creates the actor.
         * @param radius is their radius for getting hit by bullets and stuff.
         * @param rat    is the spritesheet to draw them with.
         */
        Actor(float radius, Rat rat);
    
        /**
         * Does some more update stuff that is only specific to actors.
         */
        void actorUpdate();
};

#endif
