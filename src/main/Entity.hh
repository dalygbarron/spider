#ifndef ENTITY_H
#define ENTITY_H

#include "Mesh.hh"
#include "Rectangle.hh"
#include "filesystem.hh"
#include "glm/vec2.hpp"

/**
 * Represents an entity. This is not the same as an instance of an entity that
 * you see placed within a level, this is more like the immortal concept of
 * a given entity which instances of entities are only making reference to.
 */
class Entity {
    public:
        ghc::filesystem::path file;
        std::string name;
        std::string item;
        std::string itemKey;
        std::string spriteName;
        Rectangle sprite;
        glm::vec2 offset;
        Mesh mesh;
};

#endif
