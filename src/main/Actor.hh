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
        int flip = false;
        Rat::Animation const *walkAnimation = NULL;

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
