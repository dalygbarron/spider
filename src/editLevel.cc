#include "Level.hh"
#include "Const.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int editLevel(sf::RenderWindow &window, Level &level) {
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
        if(ImGui::Begin(level.file.c_str())) {
            if (ImGui::Button(level.clean ? "Save" : "+Save+")) {
                // TODO: stuff
            }
            if (ImGui::Button("Select Pic")) fileDialog.Open();
            ImGui::SameLine();
            Picture *picture = level.getPicture();
            if (picture) {
                ImGui::Text(
                    "Current Picture: %s",
                    picture->path.c_str()
                );
            }
            ImGui::Separator();
            ImGui::Text("Entities");
            ImGui::BeginChild("Scrolling", ImVec2(150, 0), true);
            static int selected = -1;
            static float longitude = 0;
            static float latitude = 0;
            for (int n = 0; n < 50; n++) {
                if (ImGui::Selectable("Bongonr", selected == n)) selected = n;
            }
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::BeginChild("itemView",ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            ImGui::Text("Number: %d", selected);
            ImGui::DragFloat("longitude", &longitude, 0.01f, -Const::PI, Const::PI, "%.2f");
            ImGui::DragFloat("latitude", &latitude, 0.01f, -Const::PI, Const::PI, "%.2f");
            ImGui::Separator();
            ImGui::EndChild();
            ImGui::EndGroup();
            ImGui::End();
        }
        // File selector.
        fileDialog.Display();
        if (fileDialog.HasSelected()) {
            window.setTitle(fileDialog.GetSelected().string());
            fileDialog.ClearSelected();
        }
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
}
