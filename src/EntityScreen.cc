#include "Screen.hh"
#include "Const.hh"
#include "FileIO.hh"

EntityScreen::EntityScreen(Core &core, Entity &entity):
    Screen(core),
    entity(entity)
{
    this->refocus();
    this->background = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    this->spriteBuffer[0] = 0;
}

EntityScreen::~EntityScreen() {
    // TODO: stuff I suppose.
}

Screen *EntityScreen::update(float delta, sf::RenderWindow &window) {
    ImGui::SFML::Update(window, sf::seconds(delta));
    if (ImGui::Begin("Entity")) {
        if (ImGui::Button("save")) {
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
        if (ImGui::Button("refocus")) this->refocus();
        ImGui::InputText("Sprite", this->spriteBuffer, SPRITE_BUFFER_SIZE);
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
}

void EntityScreen::onDrag(sf::Mouse::Button button, sf::Vector2f delta) {
    if (button == sf::Mouse::Button::Right) {
        this->camera.x += delta.x;
        this->camera.y += delta.y;
    }
}

void EntityScreen::onScroll(int delta) {
    this->camera.z += delta * 0.1;
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
    this->core.renderer.batch.draw(
        this->sprite,
        sf::Vector2f(this->camera.x, this->camera.y),
        0,
        sf::Vector2f(this->camera.z, this->camera.z)
    );
    this->core.renderer.box(sf::FloatRect(
        this->camera.x - this->sprite.width / 2 * this->camera.z,
        this->camera.y - this->sprite.height / 2 * this->camera.z,
        this->sprite.width * this->camera.z,
        this->sprite.height * this->camera.z
    ), false);
    target.draw(this->core.renderer.batch);
    ImGui::SFML::Render(target);
}
