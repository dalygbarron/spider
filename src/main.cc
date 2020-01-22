#include "Level.hh"
#include "Entity.hh"
#include "Options.hh"
#include "Util.hh"
#include "Screen.hh"
#include "Const.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/System.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Prints out the version of the program to stdout.
 */
void version() {
    printf(
        "%d.%d.%d\n",
        Const::VERSION_MAJOR,
        Const::VERSION_MINOR,
        Const::VERSION_REV
    );
}

/**
 * Prints out the help documentation to stdouit.
 */
void help() {
    printf(
        Const::LOGO,
        Const::VERSION_MAJOR,
        Const::VERSION_MINOR,
        Const::VERSION_REV
    );
    printf("Usage: spider [-h] [-v] -g=gameFile [-e=entityFile| -l=levelFile]\n");
    printf(" -h means output help message and stop.\n");
    printf(" -v means output version number and stop.\n");
    printf(" -g means game file, it is required.\n");
    printf(" -e means entity file to edit, it cannot be used with -l.\n");
    printf(" -l means level file to edit, it cannot be used with -e.\n");
    printf("\nIf you run it and it says shaders are not available, it's fucked\n");
}
/**
 * Parses the commandline options.
 */
int parseOptions(Options &options, int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "hvg:e:l:")) != -1) {
        switch (opt) {
            case 'h':
                options.helpFlag = true;
                return 0;
            case 'v':
                options.versionFlag = true;
                return 0;
            case 'g':
                options.game = optarg;
                break;
            case 'e':
                options.file = optarg;
                options.entityFlag = true;
                break;
            case 'l':
                options.file = optarg;
                options.levelFlag = true;
                break;
            default:
                return 1;
        }
    }
    options.command = argv[0];
    return 0;
}

/**
 * Runs the main loop of the program over and over.
 * @param window is the window the program is running in.
 * @param view   is the view to look with.
 * @param screen is the screen of the program to start on.
 * @return the result code the program should give after.
 */
int process(sf::RenderWindow &window, sf::View &view, Screen *screen) {
    sf::Clock deltaClock;
    sf::Vector2i mouse = sf::Mouse::getPosition();
    int buttons[sf::Mouse::Button::ButtonCount];
    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) buttons[i] = 0;
    while (window.isOpen() && screen) {
        // Handle Events.
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                view = Util::getLetterboxView(
                    view,
                    sf::Vector2i(event.size.width, event.size.height)
                );
            } else if (!ImGui::GetIO().WantCaptureMouse) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    screen->onClick(event.mouseButton.button);
                    buttons[event.mouseButton.button] = true;
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    buttons[event.mouseButton.button] = false;
                } else if (event.type == sf::Event::MouseMoved) {
                    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
                        if (buttons[i]) {
                            screen->onDrag((sf::Mouse::Button)i, sf::Vector2f(
                                event.mouseMove.x - mouse.x,
                                event.mouseMove.y - mouse.y
                            ));
                        }
                    }
                    mouse.x = event.mouseMove.x;
                    mouse.y = event.mouseMove.y;
                }
            }
        }
        // Update Screen.
        sf::Time delta = deltaClock.restart();
        ImGui::SFML::Update(window, delta);
        screen->update(delta.asSeconds(), window);
        // Render.
        window.setView(view);
        window.clear();
        window.draw(*screen);
        ImGui::SFML::Render(window);
        window.display();
    }
    return 0;
}

/**
 * Start of the program.
 * @param argc is the number of arguments.
 * @param argv is the values of the arguments.
 * @return 0 iff all went well and some other number if not, currently without
 *           any meaning other than fail.
 */
int main(int argc, char **argv) {
    // Parse and validate arguments.
    Options options;
    int result = parseOptions(options, argc, argv);
    if (result != 0) return result;
    if (options.helpFlag) {
        help();
        return 0;
    }
    if (options.versionFlag) {
        version();
        return 0;
    }
    if (options.game.empty()) {
        fprintf(stderr, "Game file must be specified. -h for help.\n");
        return 1;
    }
    if (options.file.empty()) {
        fprintf(stderr, "Filename must be given. -h for help.\n");
        return 1;
    }
    // Validate the system can handle Spider.
    if (!sf::Shader::isAvailable()) {
        fprintf(stderr, "Shaders not available. Goodbye.\n");
        return 1;
    }
    // Load the data
    ghc::filesystem::path root = options.file;
    ghc::filesystem::path filename = root.filename();
    root.remove_filename();
    ghc::filesystem::current_path(root);
    Level *level;
    Entity *entity;
    if (options.levelFlag) level = Util::levelFromFile(filename);
    else entity = Util::entityFromFile(filename);
    // Set up some bits.
    sf::RenderWindow otherWindow(
        sf::VideoMode(Const::WIDTH, Const::HEIGHT),
        "bing bing wahooo"
    );
    sf::RenderWindow window(
        sf::VideoMode(Const::WIDTH, Const::HEIGHT),
        Const::TITLE
    );
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;
    window.resetGLStates();
    sf::View view;
    view.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    view.setCenter(sf::Vector2f(Const::WIDTH / 2, Const::HEIGHT / 2));
    // set up the first screen.
    Screen *screen = NULL;
    if (options.levelFlag) screen = new LevelScreen(level);
    else screen = new EntityScreen(entity);
    result = process(window, view, screen);
    // Clean up.
    ImGui::SFML::Shutdown();
    return result;
}
