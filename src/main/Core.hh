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
        // TODO: some of these things ought to be const.
        std::string name;
        std::string start;
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

        /**
         * Puts a screen onto the screen stack.
         * @param screen is the screen to put on there.
         */
        void pushScreen(Screen *screen);

        /**
         * Removes the current top screen from the screen stack and deletes it.
         * @param response is a value to give to the new top screen.
         */
        void popScreen(int response);

        /**
         * Removes the current top screen from the screen stack and replaces it
         * with another screen. The old screen is deleted.
         * @param screen is the new top screen on the stack.
         */
        void replaceScreen(Screen *screen);

        /**
         * Gives you the top screen in the screen stack if there is one.
         * @return a pointer to the top screen or null if there are no screens.
         */
        Screen *getTopScreen();

        /**
         * Draw all the screens one by one.
         * @param target is the target to draw to.
         */
        void drawScreens(sf::RenderTarget &target);

    private:
        sf::Font *font;
        std::vector<Screen *> screens;
        int firstVisible = 0;
        int nScreens = 0;
};

#endif
