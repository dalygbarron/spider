#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include "main/Level.hh"
#include "main/Entity.hh"
#include "main/Options.hh"
#include "main/Util.hh"
#include "main/FileIO.hh"
#include "main/Screen.hh"
#include "main/Const.hh"
#include "main/imgui.h"
#include "main/imgui-SFML.h"
#include "main/spdlog/spdlog.h"
#include "main/spdlog/rotating_file_sink.h"
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
    printf("Usage: spider [-h] [-v] [-g=gameFile] -r|-e=entityFile|-l=levelFile\n");
    printf(" -h means output help message and stop.\n");
    printf(" -v means output version number and stop.\n");
    printf(" -g means game file, it is required.\n");
    printf(" -e means entity file to edit, it cannot be used with -l, and it is relative to the game file.\n");
    printf(" -l means level file to edit, it cannot be used with -e, and it is relative to the game file\n");
    printf(" -r means open in rat mode.\n");
    printf("\nIf you run it and it says shaders are not available, it's fucked\n");
}
/**
 * Parses the commandline options.
 */
int parseOptions(Options &options, int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "hvrg:e:l:")) != -1) {
        switch (opt) {
            case 'h':
                options.helpFlag = true;
                return 0;
            case 'v':
                options.versionFlag = true;
                return 0;
            case 'r':
                options.ratFlag = true;
                break;
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
    return 0;
}

/**
 * Runs the main loop of the program over and over.
 * @param screen is the screen of the program to start on.
 * @return the result code the program should give after.
 */
int process(Screen *screen) {
    if (!screen) {
        spdlog::critical("No Screen Created. Aborting.");
        return 1;
    }
    sf::RenderWindow window(
        sf::VideoMode(Const::WIDTH, Const::HEIGHT),
        Const::TITLE// TODO: add to core and get from core.
    );
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;
    window.resetGLStates();
    sf::View view;
    view.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    view.setCenter(sf::Vector2f(Const::WIDTH / 2, Const::HEIGHT / 2));
    sf::Clock deltaClock;
    sf::Clock fps;
    int frame = 0;
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
                    sf::Vector2u size = window.getSize();
                    screen->onClick(event.mouseButton.button, sf::Vector2f(
                        event.mouseButton.x * ((float)Const::WIDTH / size.x),
                        event.mouseButton.y * ((float)Const::HEIGHT / size.y)
                    ));
                    buttons[event.mouseButton.button] = true;
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    buttons[event.mouseButton.button] = false;
                } else if (event.type == sf::Event::MouseMoved) {
                    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
                        if (buttons[i]) {
                            sf::Vector2u size = window.getSize();
                            screen->onDrag(
                                (sf::Mouse::Button)i,
                                sf::Vector2f(
                                    (event.mouseMove.x - mouse.x) *
                                        ((float)Const::WIDTH / size.x),
                                    (event.mouseMove.y - mouse.y) *
                                        ((float)Const::HEIGHT / size.y)
                                ),
                                sf::Vector2f(
                                    mouse.x * ((float)Const::WIDTH / size.x),
                                    mouse.y * ((float)Const::HEIGHT / size.y)
                                )
                            );
                        }
                    }
                    mouse.x = event.mouseMove.x;
                    mouse.y = event.mouseMove.y;
                } else if (event.type == sf::Event::MouseWheelScrolled) {
                    screen->onScroll(event.mouseWheelScroll.delta);
                } else if (event.type == sf::Event::KeyPressed) {
                    screen->onKey(event.key.code);
                }
            }
        }
        // Update Screen.
        sf::Time delta = deltaClock.restart();
        screen->update(delta.asSeconds(), window);
        // Render.
        window.setView(view);
        window.draw(*screen);
        window.display();
        // frame rate.
        frame++;
        if (fps.getElapsedTime().asSeconds() > 4) {
            spdlog::info("FPS: {}", (float)frame / 4);
            fps.restart();
            frame = 0;
        }
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
    srand(time(0));
    // Parse and validate arguments.
    Options options;
    int result = parseOptions(options, argc, argv);
    if (result != 0) return result;
    if (options.helpFlag) {
        help();
        return 0;
    } else if (options.versionFlag) {
        version();
        return 0;
    } else if (options.entityFlag && options.levelFlag) {
        fprintf(stderr, "Cannot edit entity and level at same time.\n");
        return 1;
    } else if (!sf::Shader::isAvailable()) {
        fprintf(stderr, "Shaders not available. Goodbye.\n");
        return 1;
    }
    // Change directory to be at game core file.
    ghc::filesystem::path root = options.game;
    ghc::filesystem::path coreFile = options.game.filename();
    root.remove_filename();
    ghc::filesystem::current_path(root);
    Core *core = FileIO::loadCoreFromFile(coreFile);
    // set up the first screen.
    Screen *screen = NULL;
    Level *level = NULL;
    Entity *entity = NULL;
    if (options.ratFlag) {
        screen = new RatScreen(*core);
    } else if (options.levelFlag) {
        Level *level = FileIO::levelFromFile(options.file);
        screen = new LevelScreen(*core, *level);
    } else {
        Entity *entity = FileIO::entityFromFile(options.file);
        screen = new EntityScreen(*core, *entity);
    }
    result = process(screen);
    // Clean up.
    ImGui::SFML::Shutdown();
    delete core;
    return result;
}
