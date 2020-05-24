#ifndef CREATURE_H
#define CREATURE_H

/**
 * Like an entity but for things that move around in the 3D world and have
 * behaviours and stuff.
 */
class Creature {
    public:
        bool edible;
        Rat rat;
        Rat::Animation front;
        Rat::Animation back;
        glm::vec2 offset;
};

#endif
