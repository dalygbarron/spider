#include "Level.hh"
#include "Entity.hh"
#include "Options.hh"
#include "Util.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int editLevel(sf::RenderWindow &window, Level &level);

int editEntity(sf::RenderWindow &window, Entity &entity);

int parseOptions(Options &options, int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "he")) != -1) {
        switch (opt) {
            case 'h':
                options.helpFlag = true;
                break;
            case 'e':
                options.levelFlag = false;
                break;
            default:
                return 1;
        }
    }
    int extras = argc - optind;
    if (extras == 0) {
        fprintf(stderr, "File name must be passed\n");
        return 1;
    } else if (extras > 1) {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    }
    options.file = argv[optind];
    options.command = argv[0];
    return 0;
}

int main(int argc, char **argv) {
    // Parse and validate arguments.
    Options options;
    int result = parseOptions(options, argc, argv);
    if (result != 0) return result;
    if (options.helpFlag) {
        printf("Help: Just use the program you bloody idiot\n");
        return 0;

    }
    if (options.file.empty()) {
        fprintf(stderr, "Filename must be given\n");
        return 1;
    }
    options.file.append("../bongo.png");
    printf("%s\n", options.file.c_str());
    // Load the data
    Level *level;
    Entity *entity;
    if (options.levelFlag) {
        level = Util::levelFromFile(options.file);
    } else {
        entity = Util::entityFromFile(options.file);
    }
    // Set up some bits.
    sf::RenderWindow window(sf::VideoMode(640, 480), "Spider");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;
    window.resetGLStates();
    // Run.
    if (options.levelFlag) result = editLevel(window, *level);
    else result = editEntity(window, *entity);
    // Clean up.
    ImGui::SFML::Shutdown();
    return result;
}
