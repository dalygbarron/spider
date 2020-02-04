#ifndef SCREEN_H
#define SCREEN_H

#include "Level.hh"
#include "Entity.hh"
#include "Core.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
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
         */
        virtual void onDrag(
            sf::Mouse::Mouse::Button button,
            sf::Vector2f delta
        );

        /**
         * Called when the user does the scrolling.
         * @param delta is the number of clicks they have clicked.
         */
        virtual void onScroll(int delta);

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
         * Add an entity instance to the level.
         * @param instance is the instance to add.
         */
        void addInstance(Instance *instance);

        /**
         * Remove an entity instance from the level and free it.
         * @param index is the index of the entity to remove.
         */
        void removeInstance(int index);

        virtual Screen *update(float delta, sf::RenderWindow &window) override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta
        ) override;

    private:
        Level &level;
        sf::Vector2f camera;
        sf::Shader shader;
        sf::RectangleShape back;
        ImGui::FileBrowser backgroundSelector;
        ImGui::FileBrowser entitySelector;

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

        virtual void onScroll(int delta) override;

    private:
        char nameBuffer[EntityScreen::BUFFER_SIZE];
        char spriteBuffer[EntityScreen::BUFFER_SIZE];
        Entity &entity;
        sf::Vector3f camera;
        sf::IntRect sprite;
        std::vector<sf::CircleShape> points;
        sf::ConvexShape outline;
        sf::Color background;
        int selected = -1;

        /**
         * Put the entity back in the middle of the screen at normal size.
         */
        void refocus();

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta
        ) override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

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

#endif
