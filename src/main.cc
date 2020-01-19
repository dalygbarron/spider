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
    printf("Usage: spider [-h] [-v] [-e] file\n");
    printf(" -h means output help message and stop.\n");
    printf(" -v means output version number and stop.\n");
    printf(" -e means edit file as entity rather than level.\n");
    printf(" file is the name of the file to edit. When h or v are used it is not needed.\n");
}
/**
 * Parses the commandline options.
 */
int parseOptions(Options &options, int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "hev")) != -1) {
        switch (opt) {
            case 'h':
                options.helpFlag = true;
                return 0;
            case 'v':
                options.versionFlag = true;
                return 0;
            case 'e':
                options.levelFlag = false;
                break;
            default:
                return 1;
        }
    }
    int extras = argc - optind;
    if (extras == 0) {
        fprintf(stderr, "File name must be given. -h for help.\n");
        return 1;
    } else if (extras > 1) {
        fprintf(stderr, "Too many arguments. -h for help.\n");
        return 1;
    }
    options.file = argv[optind];
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
    std::vector<Screen *> screens;
    screens.add(screen);
    while (screens.size() > 0) {
        std::vector<Screen *>::iterator iterator = screens.begin();
        while (*it != screens.end()) {
            Screen::Transition transition = screens[i].update();
            if (transition.screen) screens.push_back(transition.screen);
            if (transition.kill) iterator = screens.erase(iterator);
            else iterator++;
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
    if (options.file.empty()) {
        fprintf(stderr, "Filename must be given\n");
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
    // set up the first screen.
    Screen *screen = NULL;
    if (options.levelFlag) screen = new LevelScreen(level);
    else screen = new EntityScreen();
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;
    window.resetGLStates();
    sf::View view;
    view.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    view.setCenter(sf::Vector2f(Const::WIDTH / 2, Const::HEIGHT / 2));
    result = process(window, view, screen);
    // Clean up.
    ImGui::SFML::Shutdown();
    return result;
}
