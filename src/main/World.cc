#include "World.hh"
#include "Const.hh"
#include "Shaders.hh"
#include "spdlog/spdlog.h"

World::World(
    sf::Texture const *ground,
    sf::Color horizon,
    sf::Color bottomSky,
    sf::Color topSky,
    float waves,
    glm::ivec2 size,
    glm::vec2 fov
): background(Rectangle(glm::vec2(), size)) {
    this->position.x = 0;
    this->position.y = 1;
    this->position.z = 0;
    this->gravity.x = 0;
    this->gravity.y = 0;
    this->gravity.z = 0;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0.09;
    this->background.initFromString(Shaders::WORLD_SHADER);
    this->background.setTexture(ground);
    this->background.setUniform("fov", fov);
    this->background.setUniform("position", this->position);
    this->background.setUniform("waves", waves);
}

std::pair<char const *, char const *> World::update(glm::mat4 const &c) {
    // TODO: make camera and position into one 4x4 matrix
    char const *function = NULL;
    char const *argument = NULL;
    // Update the backgorund.
    this->background.setUniform("camera", glm::inverse(c));
    this->background.setUniform("position", this->position);
    this->background.update();
    // Update the lindels.
    for (Lindel &lindel: this->lindels) {
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    if (this->position.y < 0.5) this->velocity.y = 0.04;
    // Update the player.
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
    // Return something.
    return std::pair(function, argument);
}

void World::draw(
    sf::RenderTarget &target,
    Renderer &renderer,
    glm::mat4 const &c
) const {
    // Draw the background.
    this->background.draw(target);
    // Draw the lindels.
    for (Lindel const &lindel: this->lindels) {
        if (!lindel.alive) continue;
        glm::vec4 p = c * glm::vec4(lindel.position, 0);
        if (p.w < 0) continue;
        float scale = p.w;
        p = p / p.w;
        glm::vec2 screen = glm::vec2(p.x + 1, 1 - p.y) * 0.5f * glm::vec2(1008, 672);
        renderer.batch.draw(
            lindel.entity.sprite,
            screen,
            lindel.entity.offset,
            0,
            glm::vec2(1, 1)
        );
    }
}

void World::addLindel(Entity const &entity, glm::vec3 position) {
    Lindel lindel(entity);
    lindel.position = position;
    lindel.alive = true;
    this->lindels.push_back(lindel);
}
