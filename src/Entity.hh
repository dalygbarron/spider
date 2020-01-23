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
        sf::Vector2f origin;
        std::vector<sf::Vector2f> shape;

        /**
         * Creates the entity and gives it a picture.
         * @param picture is the entity's picture.
         */
        Entity();

        /**
         * Destroy's the entity and it's picture.
         */
        ~Entity();
};

#endif
