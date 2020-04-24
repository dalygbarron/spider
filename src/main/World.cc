#include "World.hh"
#include "Const.hh"
#include "spdlog/spdlog.h"

World::World(
    sf::Texture const *ground,
    sf::Color horizon,
    sf::Color bottomSky,
    sf::Color topSky
): background(sf::IntRect(0, 0, Const::WIDTH, Const::HEIGHT)) {
    this->background.initFromString(BACKGROUND_SHADER);
    this->background.setTexture(ground);
    this->background.setUniform("horizon", horizon);
    this->background.setUniform("bottomSky", bottomSky);
    this->background.setUniform("topSky", topSky);
    this->position.z = 1;
    this->velocity.y = -0.03;
    this->gravity.z = -0.02;
}

std::pair<char const *, char const *> World::update(glm::mat4 const &c) {
    // TODO: make camera and position into one 4x4 matrix
    this->background.setUniform("camera", c);
    this->background.setUniform("position", this->position);
    this->background.update();
    char const *function = NULL;
    char const *argument = NULL;
    // Update the lindels.
    for (Lindel &lindel: this->lindels) {
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    if (this->position.z < 0.5) this->velocity.z = 0.3;
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
    this->background.draw(target);
    // Draw the lindels.
    for (Lindel const &lindel: this->lindels) {
        if (!lindel.alive) continue;
        glm::vector4 screen = glm::vec4(
            lindel.position.x,
            lindel.position.y,
            lindel.position.z,
            1
        ) * c;
        float scale = 1;
        renderer.batch.draw(
            lindel.entity.sprite,
            sf::Vector2f(screen.x, screen.y),
            lindel.entity.offset,
            0,
            sf::Vector2f(scale, scale)
        );
    }
}

void World::addLindel(Entity const &entity, sf::Vector3f position) {
    Lindel lindel(entity);
    lindel.position = position;
    lindel.alive = true;
    this->lindels.push_back(lindel);
}
