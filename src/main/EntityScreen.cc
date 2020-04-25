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
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

void EntityScreen::update(sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(0));
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
        for (std::unordered_map<std::string, Rectangle>::const_iterator it =
            this->core.spritesheet.begin(); it != this->core.spritesheet.end();
            it++
        ) {
            if (!this->spriteBuffer[0] ||
                it->first.find(this->spriteBuffer) != std::string::npos
            ) {
                if (ImGui::Button(it->first.c_str())) {
                    this->entity.spriteName = it->first;
                    this->entity.sprite = it->second;
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void EntityScreen::onDrag(glm::ivec2 delta, glm::ivec2 pos) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        glm::vec2 *vertex = this->entity.mesh.getVertex(this->selected);
        if (vertex) {
            vertex->x += delta.x / this->camera.z;
            vertex->y += delta.y / this->camera.z;
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        this->camera.x += delta.x;
        this->camera.y += delta.y;
    }
}

void EntityScreen::onClick(sf::Mouse::Button button, glm::ivec2 pos) {
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
    this->camera = glm::vec3(this->core.getSize() / 2, 1);
}

void EntityScreen::draw(sf::RenderTarget &target, int top) const {
    target.clear(this->background);
    this->core.renderer.batch.clear();
    glm::vec2 cameraPos(this->camera.x, this->camera.y);
    // Draw the sprite.
    this->core.renderer.batch.draw(
        this->entity.sprite,
        cameraPos,
        glm::vec2(),
        0,
        cameraPos
    );
    // Draw the bounds of the sprite.
    this->core.renderer.box(Rectangle(
        cameraPos - (glm::vec2)this->entity.sprite.size *
            glm::vec2(this->camera.z / 2, this->camera.z / 2),
        (glm::vec2)this->entity.sprite.size * this->camera.z
    ), false);
    // Draw the Outline.
    std::vector<glm::vec2> const &vertices = this->entity.mesh.getVertices();
    int n = vertices.size();
    for (int i = 1; i < n; i++) {
        this->core.renderer.club(
            glm::vec2(
                vertices[i - 1].x * this->camera.z + this->camera.x,
                vertices[i - 1].y * this->camera.z + this->camera.y
            ),
            glm::vec2(
                vertices[i].x * this->camera.z + this->camera.x,
                vertices[i].y * this->camera.z + this->camera.y
            ),
            this->selected == i - 1
        );
    }
    if (n > 0) {
        this->core.renderer.club(
            glm::vec2(
                vertices[n - 1].x * this->camera.z + this->camera.x,
                vertices[n - 1].y * this->camera.z + this->camera.y
            ),
            glm::vec2(
                vertices[0].x * this->camera.z + this->camera.x,
                vertices[0].y * this->camera.z + this->camera.y
            ),
            this->selected == n - 1
        );
    }
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}
