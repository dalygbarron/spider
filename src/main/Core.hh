#ifndef CORE_H
#define CORE_H

#include "Repository.hh"
#include "RatPack.hh"
#include "Renderer.hh"
#include "RatBatch.hh"

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        std::string name;
        ghc::filesystem::path filename;
        RatPack spritesheet;
        Renderer renderer;
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;

        /**
         * Initialises the core. Fonts must be passed in because they don't
         * work until they have been loaded.
         * @param font             is the main font the game will use.
         */
        Core(sf::Font *font);

        /**
         * Deletes stuff.
         */
        ~Core();

        /**
         * Gives you the font.
         * @return the font.
         */
        sf::Font &getFont();

        /**
         * Loads in a level making use of the entity repository for putting in
         * it's entities. There is no repository for levels because they use
         * a lot of memory.
         * @param path is the file from which to load the level.
         * @return the level, probably, or null if it was in some way fucked.
         */
        Level *loadLevel(ghc::filesystem::path const &path);

    private:
        sf::Font *font;
};

#endif
