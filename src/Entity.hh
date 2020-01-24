#ifndef ENTITY_H
#define ENTITY_H

#include "filesystem.hh"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * Represents an entity.
 */
class Entity {
    public:
        ghc::filesystem::path file;
        std::string name;
        std::string item;
        std::string sprite;
        sf::Vector2f origin;
        std::vector<sf::Vector2f> shape;
};

#endif
