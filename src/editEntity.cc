#include "Entity.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int editEntity(sf::RenderWindow &window, Entity &entity) {
    // Make some tihngies.
    ImGui::FileBrowser fileDialog;
    fileDialog.SetTitle("bastard");
    fileDialog.SetTypeFilters({ ".h", ".cpp" });
    sf::Clock deltaClock;
    // Main loop.
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        // Now do the gui.
        if(ImGui::Begin("dummy window"))
        {
            if(ImGui::Button("open file dialog")) fileDialog.Open();
        }
        ImGui::End();
        fileDialog.Display();
        if(fileDialog.HasSelected())
        {
            window.setTitle(fileDialog.GetSelected().string());
            fileDialog.ClearSelected();
        }
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
}
