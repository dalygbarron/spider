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
         * Represents a message from this screen back to the main program
         * telling it to do nothing or something to do with creating more
         * screens and that.
         */
        class Transition {
            public:
                Screen *screen;
                int kill;

                /**
                 * Creates a transition.
                 * @param screen is the screen to create. Can be null to do
                 *               nothing at all.
                 * @param kill   means to kill this screen if true.
                 */
                Transition(Screen *screen, int kill);
        };

        /**
         * Does stuff like initialising the window that the screen operates
         * within.
         */
        Screen();

        /**
         * lets the screen subtypes be deleted.
         */
        virtual ~Screen();

        /**
         * Gives you access to the screen's window.
         * @return the window.
         */
        sf::RenderWindow &getWindow();

        /**
         * Updates the status of the screen.
         * @return a transition object which can contain a new screen to create
         *         or an instruction to remove this scene or whatever.
         */
        virtual Screen::Transition update();

    private:
        sf::Clock deltaClock;
        sf::Vector2i mouse;
        int buttons[sf::Mouse::Button::ButtonCount];
        sf::RenderWindow window;
        sf::View view;

        /**
         * This is where the screen implements it's actual logic.
         * @param delta is the amount of time passing that it should simulate.
         * @return a transition for what happens after this.
         */
        virtual Screen::Transition logic(float delta) = 0;

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

    private:
        Level *level;
        sf::Vector2f camera;
        sf::Shader shader;
        sf::RectangleShape back;
        ImGui::FileBrowser backgroundSelector;
        ImGui::FileBrowser entitySelector;

        virtual Screen::Transition logic(float delta) override;

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;

        virtual void onClick(sf::Mouse::Button button) override;

        virtual void onDrag(
            sf::Mouse::Button button,
            sf::Vector2f delta
        ) override;
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

    private:
        virtual Screen::Transition logic(float delta) override;

        virtual void draw(
            sf::RenderTarget &target,
            sf::RenderStates states
        ) const override;
};

#endif
