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
         * Tells if you if the level is currently clean or if it has changes
         * since it was last saved.
         * @return true if it is clean and false if it is not clean.
         */
        int getClean() const;

        /**
         * Gives you the level's picture.
         * @return a reference to the picture.
         */
        Picture const *getPicture() const;

        /**
         * Sets the picture that the level should use and deletes the old one.
         * @param picture is the picture to use which the level then takes
         *        sole ownership of.
         */
        void setPicture(Picture *picture);

    private:
        int clean = false;
        Picture *picture = NULL;
};

#endif
