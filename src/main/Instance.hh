#ifndef INSTANCE_H
#define INSTANCE_H

#include "Entity.hh"
#include "Mesh.hh"
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
        Entity const *entity;
        sf::Vector2f pos;
        Mesh mesh;
        float size = 1;
};

#endif
