#ifndef SCREEN_H
#define SCREEN_H

#include "Level.hh"
#include "Entity.hh"
#include "Core.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include "TextEditor.h"
#include <SFML/Graphics.hpp>

/**
 * A screen that has different functionality ya know.
 */
class Screen: public sf::Drawable {
    public:
        /**
         * Creates the screen and puts in it's main dependencies.
         * @param core contains the main dependencies.
         */
        Screen(Core &core);

        /**
         * lets the screen subtypes be deleted.
         */
        virtual ~Screen();

        /**
         * Updates the status of the screen.
         * @param delta  is the amount of time's passage to take into
         *               consideration.
         * @param window is the window so that you can mess around with it and
         *               get stuff that is needed.
         * @return the screen that the next frame should consist of, so
         *         normally it will return itself but sometimes it will return
         *         a different screen which should then be transitioned to. If
         *         it returns null then that means the program should now
         *         close.
         */
        virtual Screen *update(float delta, sf::RenderWindow &window) = 0;

        /**
         * Called when the user clicks on the screen.
         * @param button is the button that they clicked.
         */
        virtual void onClick(sf::Mouse::Button button, sf::Vector2f pos);

        /**
         * Called when the user drags the mouse on the screen.
         * @param button is the button they are dragging with.
         * @param delta  is the movement they have made with the mouse.
         * @param pos    is the new location of the screen.
         */
        virtual void onDrag(
            sf::Mouse::Mouse::Button button,
            sf::Vector2f delta,
            sf::Vector2f pos
        );

        /**
         * Called when the user does the scrolling.
         * @param delta is the number of clicks they have clicked.
         */
        virtual void onScroll(int delta);

        /**
         * Called when the user presses a key.
         * @param key is the key that was pressed.
         */
        virtual void onKey(sf::Keyboard::Key key);

    protected:
        Core &core;

    private:
        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override = 0;
};

/**
 * Screen in which a level is edited.
 */
class LevelScreen: public Screen {
    public:
        static int const NAME_BUFFER_SIZE = 64;

        /**
         * Creates the level screen and gives it it's level. It then becomes
         * responsible for this level and destoys it when it is done.
         * @param core  is the main dependencies of screens.
         * @param level is the level to edit.
         */
        LevelScreen(Core &core, Level &level);

        /**
         * Destructor which does little to noghitn.
         */
        ~LevelScreen();

        /**
         * Gives you the instance with the given name.
         * @param name is the name of the instance to get.
         * @return the instance if there is one or null.
         */
        Instance const *getInstanceWithName(char const *name) const;

        /**
         * Remove an entity instance from the level and free it.
         * @param index is the index of the entity to remove.
         */
        void removeInstance(int index);

        /**
         * Creates an instance in the level and returns a reference to it.
         * @param entity is what to set the instance's entity field to, if you
         *               choose null, that is ok because it means it is a shape
         *               instance not an entity instance.
         * @return the created instance.
         */
        Instance &addInstance(Entity const *entity);

        virtual Screen *update(float delta, sf::RenderWindow &window) override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onKey(sf::Keyboard::Key key) override;

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta,
            sf::Vector2f pos
        ) override;

        virtual void onScroll(int delta) override;

    private:
        Level &level;
        Instance *selectedInstance = NULL;
        int selected;
        int selectedEntity;
        std::vector<std::string> entities;
        sf::Vector2f camera;
        sf::Shader shader;
        sf::RectangleShape back;
        sf::Texture texture;
        TextEditor textEditor;
        ImGui::FileBrowser backgroundSelector;
        ImGui::FileBrowser entitySelector;

        /**
         * Loads an entity into the entity repository and at the same time
         * saves it's key to the level so that it remembers that it is a thing
         * for it.
         * @param key is the key of the entity to load.
         * @return whatever the entity repo loaded.
         */
        Entity *loadEntity(char const *key);

        /**
         * Does the menu for when an entity is selected.
         */
        void entityMenu();

        /**
         * Does the menu for when a shape is selected.
         */
        void shapeMenu();

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

/**
 * Screen in which an entity is edited.
 */
class EntityScreen: public Screen {
    public:
        static int const BUFFER_SIZE = 128;

        /**
         * Creates the screen.
         * @param core  is the main dependencies of screens.
         * @param entity is the entity that the screen shall edit.
         */
        EntityScreen(Core &core, Entity &entity);

        /**
         * Frees the entity.
         */
        virtual ~EntityScreen();

        virtual Screen *update(float delta, sf::RenderWindow &window) override;

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta,
            sf::Vector2f pos
        ) override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onScroll(int delta) override;

        virtual void onKey(sf::Keyboard::Key key) override;

    private:
        char nameBuffer[EntityScreen::BUFFER_SIZE];
        char spriteBuffer[EntityScreen::BUFFER_SIZE];
        Entity &entity;
        sf::Vector3f camera;
        sf::Color background;
        int selected = -1;

        /**
         * Put the entity back in the middle of the screen at normal size.
         */
        void refocus();

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

/**
 * Screen that shows a bunch of stuff from the ratpack onto the screen just for
 * a nice little test.
 */
class RatScreen: public Screen {
    public:
        /**
         * A little test class that moves about.
         */
        class Rat {
            public:
                float rotation;
                float angularVelocity;
                sf::IntRect sprite;
                sf::Vector2f position;
                sf::Vector2f velocity;
                sf::Vector2f gravity;
                sf::Vector2f scale;

                /**
                 * Updates the velocity and position.
                 * @param delta is the amount of time to move along.
                 */
                void update(float delta);
        };

        /**
         * Creates the screen.
         * @param core is the core stuff which contains the ratpack.
         */
        RatScreen(Core &core);

        virtual Screen *update(float delta, sf::RenderWindow &window) override;

    private:
        std::vector<Rat> rats;
        Mesh mesh;

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

/**
 * Screen where the player plays adventure game type bits of the game.
 */
class AdventureScreen: public Screen {
    public:
        /**
         * Creates the screen by giving it it's dependencies.
         * @param core  is the core screen dependencies.
         * @param level is the level the screen takes place in.
         */
        AdventureScreen(Core &core, Level const &level);

        virtual Screen *update(float delta, sf::RenderWindow &window) override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onKey(sf::Keyboard::Key key) override;

    private:
        Level const &level;
        sf::Vector2f camera;
        sf::Shader shader;
        sf::RectangleShape back;

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
