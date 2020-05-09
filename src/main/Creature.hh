#ifndef CREATURE_H
#define CREATURE_H

/**
 * Like an entity but for things that move around in the 3D world and have
 * behaviours and stuff.
 */
class Creature {
    public:
        bool edible;
        Rectangle frontRat;
        Rectangle backRat;
        glm::vec2 offset;

};

#endif
