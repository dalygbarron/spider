#ifndef ENTITY_H
#define ENTITY_H

#include "Picture.hh"
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
        Entity(Picture *picture);

        /**
         * Destroy's the entity and it's picture.
         */
        ~Entity();

        /**
         * Sets the entity's picture.
         * @param picture is the picture to set it to.
         */
        void setPicture(Picture *picture);

        /**
         * Gives you the entity's picture pointer.
         * @return the picture pointer which may be null.
         */
        Picture *getPicture();

    private:
        Picture *picture;
};

#endif
