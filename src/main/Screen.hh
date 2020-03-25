#ifndef SCREEN_H
#define SCREEN_H

#include "Level.hh"
#include "Entity.hh"
#include "Core.hh"
#include "Knob.hh"
#include "Pool.hh"
#include "Bullet.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include "TextEditor.h"
#include "sol.hh"
#include <SFML/Graphics.hpp>

/**
 * A screen that has different functionality ya know.
 */
class Screen {
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
         */
        virtual void update(float delta, sf::RenderWindow &window) = 0;

        /**
         * Draw the screen.
         * @param target is the window to draw it to.
         * @param top    is whether this screen is being drawn on the top.
         */
        virtual void draw(sf::RenderTarget &target, int top) const = 0;

        /**
         * Tells you if the screen under this one in the screen stack can be
         * rendered too.
         * @return true if so.
         */
        virtual int isTransparent() const;

        /**
         * To be called when the screen is first placed onto the screen stack.
         */
        virtual void onStart();

        /**
         * Called when this screen is revealed on the screen stack, as in it
         * has been covered on the stack by another screen and now that screen
         * has been removed and this one has been revealed.
         * @param value is the value that the covering screen returned for this
         *              one to receive.
         */
        virtual void onReveal(int value);

        /**
         * Called when the user clicks on the screen.
         * @param button is the button that they clicked.
         */
        virtual void onClick(sf::Mouse::Button button, sf::Vector2f pos);

        /**
         * Called when the user drags the mouse on the screen.
         * @param prev is the previous position on the screen.
         * @param pos  is the new location of the screen.
         */
        virtual void onDrag(sf::Vector2f prev, sf::Vector2f pos);

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

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onKey(sf::Keyboard::Key key) override;

        virtual void onDrag(sf::Vector2f prev, sf::Vector2f pos) override;

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

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onDrag(sf::Vector2f prev, sf::Vector2f pos) override;

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

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

    private:
        std::vector<Rat> rats;
        Mesh mesh;
};

/**
 * Screen that displays gui knobs and lets you click on them and stuff.
 */
class KnobScreen: public Screen {
    public:
        /**
         * Creates the screen and gives it it's dependencies.
         * @param core is the core dependencies.
         * @param knob is the knob that the screen will let you interact
         *             with. If the knob has not already been baked then
         *             the screen will remove itself from the screen stack
         *             as soon as it can. Also, the knob is a pointer
         *             because when this screen is done it will delete it.
         */
        KnobScreen(Core &core, Knob *knob);

        /**
         * Deletes.
         */
        ~KnobScreen();

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual int isTransparent() const override;

    private:
        Knob *knob;
        sf::Vector2f mouse;
};

/**
 * Any screen that has a script.
 */
class ScriptedScreen: public Screen {
    public:
        /**
         * Initialises parts of the api for scripts that any scriptable screen
         * uses. Bits only a certain type of screen will use must be
         * initialised there.
         * @param core is the core dependencies.
         * @param code is the textual lua code of the script.
         */
        ScriptedScreen(Core &core, std::string const &code);

        /**
         * Same as the other constructor except it loads the script from file.
         * @param core is the core dependencies.
         * @param path is the path to the script to run.
         */
        ScriptedScreen(Core &core, ghc::filesystem::path const &path);

        /**
         * Gives you the most recent response from whatever script the screen
         * was running.
         * @return the response as an int.
         */
        int getLastResponse() const;

        /**
         * Sets the script function that the screen should be running and gets
         * it set up.
         * @param name is the name of the function to be running.
         */
        void setScript(char const *name);

        /**
         * To be called after the script has been run if there are aspects of
         * screen state that now needs to be checked.
         */
        virtual void refresh();

        /**
         * Runs the current coroutine if it is allowed.
         * @param input is the input to the coroutine.
         * @return true if the script ran.
         */
        template <class T> int runScript(T input) {
            if (!this->coroutine) return false;
            auto result = this->coroutine(input);
            if (!result.valid()) {
                sol::error error = result;
                spdlog::error("Script Error: {}", error.what());
                return false;
            } else {
                this->response = result;
            }
            this->refresh();
            return true;
        };

    protected:
        // I know it is kind of lame having them still exposed but I don't
        // really have a choice other than replicating the entire api for
        // setting stuff up.
        sol::state script;
        sol::coroutine coroutine;

    private:
        int response;

        /**
         * Sets up the stuff the script will need.
         */
        void initScript();
};

/**
 * Screen where the player plays adventure game type bits of the game.
 */
class AdventureScreen: public ScriptedScreen {
    public:
        /**
         * Creates the screen by giving it it's dependencies.
         * @param core  is the core screen dependencies.
         * @param level is the level the screen takes place in. Once you give
         *              this to the screen it will delete it when it is
         *              deleted. beware.
         */
        AdventureScreen(Core &core, Level *level);

        /**
         * Deletes the level that is stored within the screen.
         */
        ~AdventureScreen();

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onClick(
            sf::Mouse::Button button,
            sf::Vector2f pos
        ) override;

        virtual void onStart() override;

        virtual void onReveal(int response) override;

        virtual void onDrag(sf::Vector2f prev, sf::Vector2f pos) override;

        virtual void onKey(sf::Keyboard::Key key) override;

        virtual void refresh() override;

    private:
        Level *level;
        sf::Vector2f camera;
        sf::Shader shader;
        sf::RectangleShape back;
        Item const *selected = NULL;
};

/**
 * This is a screen that does a battle. It is mostly just controlled by
 * a script, but it moves bullets around and does a gui.
 */
class BattleScreen: public ScriptedScreen {
    public:
        BattleScreen(Core &core, ghc::filesystem::path const &path);

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onStart() override;

        virtual void onReveal(int response) override;

        virtual void onKey(sf::Keyboard::Key key) override;

    private:
        Pool<Bullet> bullets;
};

#endif
