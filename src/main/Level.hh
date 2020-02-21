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
        std::string script;
        std::vector<Instance *> instances;

        /**
         * Deletes the level's stuff.
         */
        ~Level();

        /**
         * sets the level's pic, both loading in the texture and setting the
         * file record.
         * @param path is the path to the pic.
         */
        void setPic(ghc::filesystem::path const &path);

        /**
         * Gives you the level's background image texture.
         * @return the texture.
         */
        sf::Texture const &getPic() const;

        /**
         * Gives you the level's background image file path.
         * @return the file path.
         */
        ghc::filesystem::path const &getPicFile() const;

        /**
         * Adds an instance to the level.
         * @param instance is a pointer to the instance which this level now
         *                 owns.
         */
        void addInstance(Instance *instance);

    private:
        sf::Texture pic;
        ghc::filesystem::path picFile;

};

#endif
