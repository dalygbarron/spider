#ifndef SCREEN_H
#define SCREEN_H

#include "Level.hh"
#include "Entity.hh"
#include "Core.hh"
#include "Rat.hh"
#include "Lindel.hh"
#include "Knob.hh"
#include "Background.hh"
#include "Pool.hh"
#include "Bullet.hh"
#include "World.hh"
#include "Limiter.hh"
#include "FileEditor.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include "TextEditor.h"
#include "sol.hh"
#include <SFML/Graphics.hpp>
#include <list>

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
         * @param delta  is the time since last frame in seconds.
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
        virtual void onClick(sf::Mouse::Button button, glm::ivec2 pos);

        /**
         * Called when the user drags the mouse on the screen.
         * @param prev is the previous position on the screen.
         * @param pos  is the new location of the screen.
         */
        virtual void onDrag(glm::ivec2 prev, glm::ivec2 pos);

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
 * A screen which contains tabs where you can select files, edit levels, edit
 * entities, and potentially even more.
 */
class EditorScreen: public Screen {
    public:
        /**
         * Creates the screen by giving it the core stuff. At the start it will
         * just contain a file browser tab.
         * @param core is the core.
         */
        EditorScreen(Core &core);

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual int isTransparent() const override;

    private:
        /**
         * Makes it start editing a given file (if possible), and updates the
         * list of active files etc.
         * @param path is the path to the file to edit.
         */
        void openFile(ghc::filesystem::path path);

        /**
         * Recursively does a whole directory tree in imgui in an existing
         * window and when you click one it sets that as the active editor if
         * possible.
         * @param path is the path that we are doing right now.
         * @param top  is whether this is the top level directory.
         */
        void directoryTree(ghc::filesystem::path path, bool top);

        /**
         * Runs the imgui file browser thing.
         */
        void fileBrowser();

        std::queue<ghc::filesystem::path> latest;
        FileEditor *active = NULL;
        float timer = 0;
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
        glm::ivec2 mouse;
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

        /**
         * Writes down user input so that the script can read it.
         */
        void sendInput();

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
        AdventureScreen(Core &core, Level::LevelInstance &level);

        /**
         * Deletes the level that is stored within the screen.
         */
        ~AdventureScreen();

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onClick(
            sf::Mouse::Button button,
            glm::ivec2 pos
        ) override;

        virtual void onStart() override;

        virtual void onReveal(int response) override;

        virtual void onDrag(glm::ivec2 prev, glm::ivec2 pos) override;

        virtual void onKey(sf::Keyboard::Key key) override;

        virtual void refresh() override;

    private:
        Level::LevelInstance &level;
        Background background;
        World *world = NULL;
        glm::vec2 angle;
        Item const *selected = NULL;
        sol::function ticker;
        Limiter limiter;
};

/**
 * This is a screen that does a battle. It is mostly just controlled by
 * a script, but it moves bullets around and does a gui.
 */
class BattleScreen: public ScriptedScreen {
    public:
        /**
         * Creates the battle screene, giving it the core, and the path to
         * a script to run the batellelllllellelel.
         * @param core is the core dependencies.
         * @param path is a path to the battle script file.
         */
        BattleScreen(Core &core, ghc::filesystem::path const &path);

        virtual void update(float delta, sf::RenderWindow &window) override;

        virtual void draw(sf::RenderTarget &target, int top) const override;

        virtual void onStart() override;

        virtual void onReveal(int response) override;

        virtual void onKey(sf::Keyboard::Key key) override;

    private:
        Pool<Bullet> bullets;
        Pool<Actor> actors;
        std::list<Rat::Animation> animations;
        Rectangle const bounds;
        Background background;
        std::string title;
        std::string subtitle;
        int frame = 0;
        unsigned int player = 0;
};

#endif
