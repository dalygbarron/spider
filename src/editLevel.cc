#include "Level.hh"
#include "Const.hh"
#include "Util.hh"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imfilebrowser.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int editLevel(sf::RenderWindow &window, sf::View view, Level &level) {
    // Make some tihngies.
    ImGui::FileBrowser fileDialog;
    fileDialog.SetTitle("bastard");
    fileDialog.SetTypeFilters({ ".h", ".cpp" });
    sf::Clock deltaClock;
    sf::Vector2f camera(0, 0);
    sf::Shader shader;
    shader.setUniform("angle", camera);
    shader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!shader.loadFromMemory(Const::SKY_SHADER, sf::Shader::Fragment)) {
        fprintf(stderr, "Couldn't start the sky shader. goodbye.\n");
        return 1;
    }
    sf::RectangleShape back(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    back.setTexture(level.getPicture()->texture);
    sf::RenderStates state(&shader);
    // Main loop.
    while (window.isOpen()) {
        camera.x += 0.01;
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
            } 
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
        // Render.
        window.setView(view);
        window.clear();
        shader.setUniform("angle", camera);
        window.draw(back, state);
        ImGui::SFML::Render(window);
        window.display();
    }
}
