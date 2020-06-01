#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#define GLM_FORCE_RADIANS
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
 * Saves the core's current screen stack render to a file with a timestamp
 * based name. If the file already exists it skips but generally it should not
 * exist.
 * @param core is the core to render.
 */
void screenshot(Core const &core) {
    sf::RenderTexture buffer;
    glm::ivec2 size = core.size;
    buffer.create(size.x, size.y);
    buffer.clear(sf::Color::Black);
    core.drawScreens(buffer);
    buffer.display();
    buffer.getTexture().copyToImage().saveToFile("bongo.png");
}

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
    printf("Usage: spider [-h] [-v] [-m] [-g=gameFile] -r|-e=entityFile|-l=levelFile|-b=battleFile\n");
    printf(" -h means output help message and stop.\n");
    printf(" -v means output version number and stop.\n");
    printf(" -m means mute the music but not the sounds.\n");
    printf(" -g means game file, it is required.\n");
    printf(" -e means entity file to edit. It is relative to the game file.\n");
    printf(" -l means level file to edit. It is relative to the game file\n");
    printf(" -r means open in rat mode.\n");
    printf(" -b means open straight into the given battle script.\n");
    printf("To play normally, provide just a game");
    printf("\nIf you run it and it says shaders are not available, it's fucked\n");
}
/**
 * Parses the commandline options.
 */
int parseOptions(Options &options, int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "hvmrg:e:l:b:")) != -1) {
        switch (opt) {
            case 'h':
                options.helpFlag = true;
                return 0;
            case 'v':
                options.versionFlag = true;
                return 0;
            case 'm':
                options.muteFlag = true;
                break;
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
            case 'b':
                options.file = optarg;
                options.battleFlag = true;
                break;
            default:
                return 1;
        }
    }
    return 0;
}

/**
 * Runs the main loop of the program over and over.
 * @param core is the core game stuff.
 * @return the result code the program should give after.
 */
int process(Core &core) {
    glm::vec2 size = core.size;
    sf::RenderWindow window(sf::VideoMode(size.x, size.y), core.name.c_str());
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    window.resetGLStates();
    sf::View view;
    view.setSize(sf::Vector2f(size.x, size.y));
    view.setCenter(sf::Vector2f(size.x / 2, size.y / 2));
    sf::Clock deltaClock;
    sf::Clock fps;
    int frame = 0;
    sf::Vector2i mouse = sf::Mouse::getPosition();
    int buttons[sf::Mouse::Button::ButtonCount];
    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) buttons[i] = 0;
    Screen *screen = core.getTopScreen();
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
                    screen->onClick(event.mouseButton.button, glm::ivec2(
                        event.mouseButton.x,
                        event.mouseButton.y
                    ));
                    buttons[event.mouseButton.button] = true;
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    buttons[event.mouseButton.button] = false;
                } else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2u size = window.getSize();
                    screen->onDrag(
                        glm::ivec2(event.mouseMove.x, event.mouseMove.y),
                        glm::ivec2(mouse.x, mouse.y)
                    );
                    mouse.x = event.mouseMove.x;
                    mouse.y = event.mouseMove.y;
                } else if (event.type == sf::Event::MouseWheelScrolled) {
                    screen->onScroll(event.mouseWheelScroll.delta);
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Key::F2) {
                        screenshot(core);
                    } else {
                        screen->onKey(event.key.code);
                    }
                }
            }
        }
        // Update Screen.
        screen->update(deltaClock.restart().asSeconds(), window);
        // Render.
        window.setView(view);
        window.clear();
        core.drawScreens(window);
        window.display();
        // frame rate.
        frame++;
        if (fps.getElapsedTime().asSeconds() > 4) {
            spdlog::info("FPS: {}", (float)frame / 4);
            fps.restart();
            frame = 0;
        }
        // Re get the screen for if there has been a transition.
        core.performTransitions();
        screen = core.getTopScreen();
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
    if (!ghc::filesystem::exists(root)) {
        fprintf(stderr, "Game file %s does not exist.\n", root.c_str());
        return 1;
    }
    root.remove_filename();
    if (!root.empty()) ghc::filesystem::current_path(root);
    Core *core = FileIO::loadCoreFromFile(coreFile, !options.muteFlag);
    // set up the first screen.
    Screen *screen = NULL;
    Level *level = NULL;
    Entity *entity = NULL;
    if (options.ratFlag) {
        core->pushScreen(new RatScreen(*core));
    } else if (options.levelFlag) {
        Level *level = core->loadLevel(options.file);
        core->pushScreen(new LevelScreen(*core, *level));
    } else if (options.entityFlag) {
        Entity *entity = core->entityRepository.get(options.file.c_str());
        core->pushScreen(new EntityScreen(*core, *entity));
    } else if (options.battleFlag) {
        core->pushScreen(new BattleScreen(*core, options.file));
    } else {
        core->pushScreen(new AdventureScreen(
            *core,
            core->loadLevel(core->start)
        ));
    }
    core->performTransitions();
    result = process(*core);
    // Clean up.
    ImGui::SFML::Shutdown();
    delete core;
    return result;
}
