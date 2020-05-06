#ifndef LINDEL_H
#define LINDEL_H

#include "Entity.hh"
#include "glm/vec3.hpp"
#include <SFML/Graphics.hpp>

/**
 * A thing that exists at a point in 3d and can hit into other stuff etc.
 */
class Lindel {
    public:
        Entity const *entity;
        Behaviour const *behaviour;
        Behaviour::State const *state;
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 gravity;
        mutable float z;
        int alive = true;

        /**
         * Constructor based on the entity that it is a rip off of.
         * @param entity    the entity that is it.
         * @param behaviour is the behaviour with which the lindel will act.
         */
        Lindel(Entity const *entity, Behaviour const *behaviour);

        /**
         * Updates the lindel's location for 1 frame.
         */
        void update();
};

#endif
