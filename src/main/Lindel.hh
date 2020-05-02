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
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 gravity;
        int alive = true;

        /**
         * Constructor based on the entity that it is a rip off of.
         * @param entity the entity that is it .
         */
        Lindel(Entity const *entity);

        /**
         * Updates the lindel's location for 1 frame.
         */
        void update();
};

#endif
