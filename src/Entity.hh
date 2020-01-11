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
        std::string name;
        std::string itemType;
        sf::Texture texture;
        sf::Vector2f origin;
        std::vector<sf::Vector2f> shape;
};

#endif
