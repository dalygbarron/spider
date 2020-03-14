#ifndef CORE_H
#define CORE_H

#include "Repository.hh"
#include "RatPack.hh"
#include "Renderer.hh"
#include "RatBatch.hh"
#include "SoundPlayer.hh"
#include "Item.hh"
#include "Memory.hh"

/**
 * Forward declaration.
 */
class Screen;

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        std::string name;
        std::string start;
        ghc::filesystem::path filename;
        RatPack spritesheet;
        Renderer renderer;
        SoundPlayer soundPlayer;
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;

        /**
         * Initialises the core.
         */
        Core();

        /**
         * Adds an item to the game, and returns a nice reference to it.
         * @param name        is the working name of the item.
         * @param displayName is the version of the name the player sees.
         * @param description is the description of the game to show.
         * @param rat         is the icon to draw the item with in inventory.
         * @return the item.
         */
        Item &addItem(
            char const *name,
            char const *displayName,
            char const *description,
            sf::IntRect rat
        );

        /**
         * Starts a new game.
         * @param id is the id to use which determines the file it is saved in
         *           and the number the player sees it as being.
         */
        void newGame(int id);

        /**
         * Loads the game from file based on the given id.
         * @param id is the id to use which determines the file it is loaded
         *           from and also the file it will save to later.
         */
        void loadGame(int id);

        /**
         * Saves the game to file with it's already set id.
         */
        void saveGame();

        /**
         * Gives you access to the overall game state ie everything that is
         * saved when you save the game.
         * @return a reference to the memory object.
         */
        Memory &getMemory();

        /**
         * Gives you the map of all the items that are in the game, with the
         * keys being their names.
         * @return a reference to the const map.
         */
        std::unordered_map<std::string, Item> const &getItems();

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
        Memory memory = Memory(0);
        std::unordered_map<std::string, Item> items;
        std::vector<Screen *> screens;
        int firstVisible = 0;
        int nScreens = 0;

        /**
         * Recalculates what screens should be rendered.
         */
        void recalculateVisible();
};

#endif
