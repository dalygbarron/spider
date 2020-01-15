#ifndef LEVEL_H
#define LEVEL_H

#include "Picture.hh"
#include "Entity.hh"
#include "Instance.hh"
#include "filesystem.hh"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

/**
 * Represents a level.
 */
class Level {
    public:
        ghc::filesystem::path file;
        std::string script;
        std::unordered_map<std::string, Entity *> defines;
        std::unordered_map<std::string, Instance *> entities;
        std::unordered_map<std::string, std::vector<sf::Vector2f>> shapes;
        int clean = false;

        /**
         * Passes in the picture that the level is using.
         * @param picture is the picture to use.
         */
        Level(Picture *picture);

        /**
         * Destroys the level's picture from memory.
         */
        ~Level();

        /**
         * Gives you the level's picture.
         * @return a reference to the picture.
         */
        Picture *getPicture();

        /**
         * Sets the picture that the level should use and deletes the old one.
         * @param picture is the picture to use which the level then takes
         *        personal ownership of.
         */
        void setPicture(Picture *picture);

    private:
        Picture *picture = NULL;
};

#endif
