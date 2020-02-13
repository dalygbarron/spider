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
        LevelRepository levelRepository;

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

    private:
        sf::Font *font;
};

#endif
