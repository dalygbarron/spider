#ifndef SCREEN_H
#define SCREEN_H

#include "Level.hh"
#include "Entity.hh"
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
        virtual Screen *update(float delta, sf::Window &window) = 0;

        /**
         * Called when the user clicks on the screen.
         * @param button is the button that they clicked.
         */
        virtual void onClick(sf::Mouse::Button button);

        /**
         * Called when the user drags the mouse on the screen.
         * @param button is the button they are dragging with.
         * @param delta  is the movement they have made with the mouse.
         */
        virtual void onDrag(
            sf::Mouse::Mouse::Button button,
            sf::Vector2f delta
        );

    private:
        sf::RenderWindow &window;

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
         * @param level is the level to edit. It should not be null. Also, the
         *              level will delete it when it is deleted.
         */
        LevelScreen(Level *level);

        /**
         * Frees the levelscreen's shit for example the level itself.
         */
        virtual ~LevelScreen();

        virtual Screen *update(float delta, sf::Window &window) override;

        virtual void onClick(sf::Mouse::Button button) override;

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta
        ) override;

    private:
        Level *level;
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
        /**
         * Frees the entity.
         */
        virtual ~EntityScreen();

        virtual Screen *update(float delta, sf::Window &window) override;

    private:
        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
