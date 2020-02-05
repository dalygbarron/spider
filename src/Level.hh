#ifndef LEVEL_H
#define LEVEL_H

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
        ghc::filesystem::path pic;
        std::string script;
        std::unordered_map<std::string, Entity *> defines;
        std::unordered_map<std::string, Instance *> entities;
        std::unordered_map<std::string, std::vector<sf::Vector2f>> shapes;

        /**
         * Tells if you if the level is currently clean or if it has changes
         * since it was last saved.
         * @return true if it is clean and false if it is not clean.
         */
        int getClean() const;

    private:
        int clean = false;
};

#endif
