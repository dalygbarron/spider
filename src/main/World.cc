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
    glm::vec2 fov,
    glm::mat4 projection
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
    this->rotation.x = 0;
    this->rotation.y = 0;
    this->size = size;
    this->fov = fov;
    this->projection = projection;
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
    this->background.setUniform(
        "camera",
        glm::inverse(c * Util::camera(this->rotation))
    );
    this->background.setUniform("position", this->position);
    this->background.update();
    // Update the lindels.
    for (Lindel &lindel: this->lindels) {
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    this->rotation.x += 0.004;
    if (this->position.y < 0.5) this->velocity.y = 0.04;
    // Update the player.
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
    // order the lindels.
    std::sort(this->lindels.begin(), this->lindels.end(), [this](
        Lindel const &a,
        Lindel const &b
    ) {
        return Util::manhattan3(a.position, this->position) >
            Util::manhattan3(b.position, this->position);
    });
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
    glm::mat4 clipper = c * Util::camera(this->rotation);
    clipper = this->projection * glm::translate(clipper, this->position);
    for (Lindel const &lindel: this->lindels) {
        if (!lindel.alive) continue;
        glm::vec4 p = clipper * glm::vec4(lindel.position, 1);
        if (p.w < 0) continue;
        float scale = 2 / p.w * lindel.entity->scale;
        p = p / p.w;
        glm::vec2 screen = glm::vec2(p.x + 1, 1 - p.y) * 0.5f * (glm::vec2)this->size;
        renderer.batch.draw(
            lindel.entity->sprite,
            screen,
            lindel.entity->offset * scale,
            0,
            glm::vec2(scale, scale)
        );
    }
}

void World::addLindel(Entity const *entity, glm::vec3 position) {
    Lindel lindel(entity);
    lindel.position = position;
    lindel.alive = true;
    this->lindels.push_back(lindel);
}
