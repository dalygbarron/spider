#ifndef CORE_H
#define CORE_H

#include "Repository.hh"
#include "Entity.hh"
#include "Level.hh"
#include "RatPack.hh"
#include "Renderer.hh"
#include "RatBatch.hh"
#include "Bullet.hh"
#include "SoundPlayer.hh"
#include "Item.hh"
#include "Memory.hh"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include <queue>

/**
 * Forward declaration.
 */
class Screen;

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        /**
         * Represents a screen transition to perform at the appropriate time.
         */
        class Transition {
            public:
                int push;
                int pop;
                int response;
                Screen *screen;
        };

        glm::ivec2 const size;
        glm::vec2 const fov;
        std::string name;
        std::string start;
        std::string defaultFont;
        ghc::filesystem::path filename;
        RatPack spritesheet;
        Renderer renderer;
        SoundPlayer soundPlayer;
        SoundBufferRepository soundBufferRepository;
        TextRepository textRepository;
        sf::Texture transitionTexture;

        /**
         * Initialises the core.
         * @param allowMusic is whether to allow music to be played.
         * @param size       is the size of the game window.
         * @param fov        is the vertical field of view.
         */
        Core(int allowMusic, glm::ivec2 size, float fov);

        /**
         * Adds an item to the game, and returns a nice reference to it.
         * @param name        is the working name of the item.
         * @param displayName is the version of the name the player sees.
         * @param description is the description of the game to show.
         * @param rat         is the icon to draw the item with in inventory.
         * @param sprite      is the actual sprite of the icon to draw.
         * @return the item.
         */
        Item &addItem(
            char const *name,
            char const *displayName,
            char const *description,
            char const *rat,
            Rectangle sprite
        );

        /**
         * Gives you the map of all the items that are in the game, with the
         * keys being their names.
         * @return a reference to the const map.
         */
        std::unordered_map<std::string, Item> const &getItems() const;

        /**
         * Creates an entity in the entity map and names it.
         * @param name is the name to give it.
         * @return a reference to the created entity.
         */
        Entity &createEntity(char const *name);

        /**
         * Gives you access to the map of all entities in read only.
         * @return reference to const map of entities.
         */
        std::unordered_map<std::string, Entity> const &getEntities() const;

        /**
         * Gives you a pointer to an entity if it exists.
         * @param name is the name of the entity you are looking for.
         * @return the entity if found or nothing.
         */
        Entity *getEntity(char const *name);

        /**
         * Creates a level in the level map and names it.
         * @param name is the name to give to it.
         * @return the created level.
         */
        Level &createLevel(char const *name);

        /**
         * Gives you a const reference to a map of all the levels.
         * @return the const reference to the map of all levels.
         */
        std::unordered_map<std::string, Level> const &getLevels() const;

        /**
         * Creates a new bullet prototype with the given id and rat. If the id
         * is already in use you get nothing.
         * @param id  is the id to give the prototype.
         * @param rat is the rat to draw the bullets of this type with.
         */
        void addBulletPrototype(int id, Rectangle rat);

        /**
         * Gives you a pointer to a given bullet prototype if it exists.
         * @param id is the id of the prototype you are looking for.
         * @return a pointer to the prototype if it exists or null if there is
         *         not any.
         */
        Bullet::Prototype const *getBulletPrototype(int id);

        /**
         * Starts a new game.
         * @param id is the id to use which determines the file it is saved in
         *           and the number the player sees it as being.
         */
        void newGame(unsigned int id);

        /**
         * Loads the game from file based on the given id.
         * @param id is the id to use which determines the file it is loaded
         *           from and also the file it will save to later.
         */
        void loadGame(unsigned int id);

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
         * Sets the strength of the transition that covers the screen.
         * If it's set to 0 then no rendering is done so there should be no
         * performance hit when it is not in use. Basically at 0 it should not
         * be shown at all, and at strength 1 it should fully mask the screen.
         * @param strength is the strength to set it to.
         */
        void setTransitionStrength(float strength);

        /**
         * Sets the colour that the transition thingy should be drawn with.
         * Does not turn it on if it is not on.
         * @param colour is the colour.
         */
        void setTransitionColour(sf::Color colour);

        /**
         * Sets the transition texture to load.
         * @param path is the name of the file to load the texture from.
         */
        void setTransitionTexture(ghc::filesystem::path const &path);

        /**
         * Gives you the game's current projection matrix based on the current
         * field of view and size.
         * @return a reference to the projection matrix.
         */
        glm::mat4 const &getProjection() const;

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
         * Goes through the current queue of transitions and does them all.
         */
        void performTransitions();

        /**
         * Draw all the screens one by one.
         * @param target is the target to draw to.
         */
        void drawScreens(sf::RenderTarget &target) const;

    private:
        Memory memory = Memory(0);
        std::queue<Core::Transition> transitions;
        std::unordered_map<std::string, Item> items;
        std::unordered_map<int, Bullet::Prototype> bulletPrototypes;
        std::unordered_map<std::string, Entity> entities;
        std::unordered_map<std::string, Level> levels;
        std::vector<Screen *> screens;
        sf::Shader transitionShader;
        sf::RectangleShape transition;
        glm::mat4 projection;
        int firstVisible = 0;
        int nScreens = 0;
        float transitionStrength = 0;
};

#endif
