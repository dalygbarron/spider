#ifndef ENTITY_H
#define ENTITY_H

#include "Mesh.hh"
#include "filesystem.hh"
#include <SFML/Graphics.hpp>
#include <vector>

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
        std::string sprite;
        sf::Vector2f origin;
        Mesh mesh;
};

#endif
