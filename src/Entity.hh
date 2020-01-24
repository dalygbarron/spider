#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * Represents an entity.
 */
class Entity {
    public:
        enum Type {
            GENERIC,
            ITEM
        } type;
        ghc::filesystem::path file;
        std::string name;
        std::string itemType;
        std::string sprite;
        sf::Vector2f origin;
        std::vector<sf::Vector2f> shape;
};

#endif
