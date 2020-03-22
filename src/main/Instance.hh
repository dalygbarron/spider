#ifndef INSTANCE_H
#define INSTANCE_H

#include "Entity.hh"
#include "Mesh.hh"
#include "Memory.hh"
#include <string>

/**
 * An instance of a thing that can appear within a level. If it has an entity
 * then it is considered to be an entity instance and the mesh is ignored, but
 * if it has no entity then it is considered a shape instance and the mesh will
 * not be ignored.
 */
class Instance {
    public:
        std::string name;
        Memory::SwitchExpression *lifeSwitch;
        Entity const *entity;
        sf::Vector2f pos;
        Mesh mesh;
        int alive = true;
        float size = 1;

        /**
         * Gives you the instance's distance from a point, taking into account
         * that the instance may consist of either a mesh or an entity. If it
         * is a mesh it will be the distance from the closest point, whereas if
         * it is an entity it will be the distance from the origin of it.
         * @param coordinate is the latitude and longitude coordinate we are
         *                   finding the distance from.
         * @return the distance.
         */
        float distance(sf::Vector2f coordinate) const;
};

#endif
