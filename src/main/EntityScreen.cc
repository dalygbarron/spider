#include "Screen.hh"
#include "Const.hh"
#include "FileIO.hh"
#include "Mesh.hh"
#include <cmath>
#include <string.h>

EntityScreen::EntityScreen(Core &core, Entity &entity):
    Screen(core),
    entity(entity)
{
    this->refocus();
    this->background = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    this->spriteBuffer[0] = 0;
    strcpy(this->nameBuffer, entity.name.c_str());
    if (!entity.sprite.empty()) {
        this->sprite = this->core.spritesheet.get(entity.sprite.c_str());
    }
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen *EntityScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::Begin("Entity")) {
        if (ImGui::Button("save")) {
            this->entity.name = this->nameBuffer;
            FileIO::saveEntity(this->entity);
        }
        ImGui::SameLine();
        if (ImGui::Button("Change Background")) {
            this->background = sf::Color(
                rand() % 255,
                rand() % 255,
                rand() % 255
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("Split") && this->selected != -1) {
            this->entity.mesh.split(this->selected);
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove") && this->selected != -1) {
            this->entity.mesh.remove(this->selected);
            if (this->selected >= this->entity.mesh.getVertices().size()) {
                this->selected--;
            }
        }
        if (ImGui::Button("refocus")) this->refocus();
        ImGui::SameLine();
        ImGui::InputText("Name", this->nameBuffer, EntityScreen::BUFFER_SIZE);
        ImGui::InputText(
            "Sprite",
            this->spriteBuffer,
            EntityScreen::BUFFER_SIZE
        );
        ImGui::BeginChild("Sprites");
        for (std::unordered_map<std::string, sf::IntRect>::const_iterator it =
            this->core.spritesheet.begin(); it != this->core.spritesheet.end();
            it++
        ) {
            if (!this->spriteBuffer[0] ||
                it->first.find(this->spriteBuffer) != std::string::npos
            ) {
                if (ImGui::Button(it->first.c_str())) {
                    this->entity.sprite = it->first;
                    this->sprite = it->second;
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
    return this;
}

void EntityScreen::onDrag(
    sf::Mouse::Button button,
    sf::Vector2f delta,
    sf::Vector2f pos
) {
    if (button == sf::Mouse::Button::Left) {
        sf::Vector2f *vertex = this->entity.mesh.getVertex(this->selected);
        if (vertex) {
            vertex->x += delta.x / this->camera.z;
            vertex->y += delta.y / this->camera.z;
        }
    } else if (button == sf::Mouse::Button::Right) {
        this->camera.x += delta.x;
        this->camera.y += delta.y;
    }
}

void EntityScreen::onClick(sf::Mouse::Button button, sf::Vector2f pos) {
    if (button == sf::Mouse::Button::Left) {
        pos.x /= this->camera.z;
        pos.y /= this->camera.z;
        pos.x -= this->camera.x / this->camera.z;
        pos.y -= this->camera.y / this->camera.z;
        this->selected = this->entity.mesh.getClosestVertex(pos, 50);
    }
}

void EntityScreen::onScroll(int delta) {
    this->camera.z += delta * 0.1;
}

void EntityScreen::onKey(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Space) {
        if (this->selected != -1) {
            this->entity.mesh.split(this->selected);
            this->selected++;
        }
    } else if (key == sf::Keyboard::Key::Backspace) {
        if (this->selected != -1 && this->entity.mesh.getVertices().size() > 2) {
            this->entity.mesh.remove(this->selected);
            this->selected--;
            if (this->selected < 0) {
                this->selected = this->entity.mesh.getVertices().size() - 1;
            }
        }
    }
}

void EntityScreen::refocus() {
    this->camera.x = Const::WIDTH / 2;
    this->camera.y = Const::WIDTH / 2;
    this->camera.z = 1;
}

void EntityScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.clear(this->background);
    this->core.renderer.batch.clear();
    // Draw the sprite.
    this->core.renderer.batch.draw(
        this->sprite,
        sf::Vector2f(this->camera.x, this->camera.y),
        0,
        sf::Vector2f(this->camera.z, this->camera.z)
    );
    // Draw the bounds of the sprite.
    this->core.renderer.box(sf::FloatRect(
        this->camera.x - this->sprite.width / 2 * this->camera.z,
        this->camera.y - this->sprite.height / 2 * this->camera.z,
        this->sprite.width * this->camera.z,
        this->sprite.height * this->camera.z
    ), false);
    // Draw the Outline.
    std::vector<sf::Vector2f> const &vertices = this->entity.mesh.getVertices();
    int n = vertices.size();
    for (int i = 1; i < n; i++) {
        this->core.renderer.club(
            sf::Vector2f(
                vertices[i - 1].x * this->camera.z + this->camera.x,
                vertices[i - 1].y * this->camera.z + this->camera.y
            ),
            sf::Vector2f(
                vertices[i].x * this->camera.z + this->camera.x,
                vertices[i].y * this->camera.z + this->camera.y
            ),
            this->selected == i - 1
        );
    }
    if (n > 0) {
        this->core.renderer.club(
            sf::Vector2f(
                vertices[n - 1].x * this->camera.z + this->camera.x,
                vertices[n - 1].y * this->camera.z + this->camera.y
            ),
            sf::Vector2f(
                vertices[0].x * this->camera.z + this->camera.x,
                vertices[0].y * this->camera.z + this->camera.y
            ),
            this->selected == n - 1
        );
    }
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}
