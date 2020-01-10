#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Spider");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    ImGui::GetIO().IniFilename = NULL;

    sf::Color bgColor(100, 200, 255);

    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
    sf::Clock deltaClock;

    ImGui::FileBrowser fileDialog;
        
    fileDialog.SetTitle("bastard");
    fileDialog.SetTypeFilters({ ".h", ".cpp" });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if(ImGui::Begin("dummy window"))
        {
            // open file dialog when user clicks this button
            if(ImGui::Button("open file dialog")) fileDialog.Open();
        }
        ImGui::End();
        
        fileDialog.Display();
        
        if(fileDialog.HasSelected())
        {
            window.setTitle(fileDialog.GetSelected().string());
            fileDialog.ClearSelected();
        }

        window.clear(bgColor);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
