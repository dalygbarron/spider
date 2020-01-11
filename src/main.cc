#include "Level.hh"
#include "Entity.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int editLevel(sf::RenderWindow &window, Level &level);

int editEntity(sf::RenderWindow &window, Entity &entity);

int main() {
    // Parse the input file.
    // TODO: this.
    Level *level = new Level();
    // Set up some bits.
    sf::RenderWindow window(sf::VideoMode(640, 480), "Spider");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;
    window.resetGLStates();
    int result = editLevel(window, *level);
    ImGui::SFML::Shutdown();
    return result;
}
