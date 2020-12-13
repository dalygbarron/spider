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
        std::string name;
        std::string script;
        std::vector<Instance> instances;

        /**
         * sets the level's pic, both loading in the texture and setting the
         * file record.
         * @param path is the path to the pic.
         */
        void setPic(ghc::filesystem::path const &path);

        /**
         * Gives you the level's background image file path.
         * @return the file path.
         */
        ghc::filesystem::path const &getPicFile() const;

        /**
         * Creates a new instance and then returns a pointer to it.
         * @return a reference to the instance.
         */
        Instance &addInstance();

    private:
        ghc::filesystem::path picFile;
};

/**
 * Represents a level with it's picture actually loaded into memory.
 */
class LevelInstance: public Level {
    public:
        sf::Texture pic;

        /**
         * Constructs a level instance by copying from a level.
         * @param level is the level to copy.
         */
        LevelInstance(Level const &level);
};

#endif
