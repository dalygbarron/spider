#include "World.hh"
#include "Const.hh"
#include "Shaders.hh"
#include "spdlog/spdlog.h"
#include "glm/gtx/fast_square_root.hpp"

World::World(
    sf::Texture const *ground,
    sf::Color horizon,
    sf::Color bottomSky,
    sf::Color topSky,
    float waves,
    glm::ivec2 size,
    glm::vec2 fov,
    glm::mat4 projection
):
    background(Rectangle(glm::vec2(), size)),
    position(0, 1, 0),
    velocity(0),
    gravity(0),
    rotation(0)
{
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
        this->lindelBehaviour(lindel);
        lindel.update();
        // TODO: collisions and that kind of thing maybe.
    }
    // Update the player.
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
    // order the lindels.
    int n = this->lindels.size();
    int portions = n / World::SORT_PHASES;
    std::vector<Lindel>::iterator start = this->lindels.begin() + portions *
        (World::SORT_PHASES - (this->phase + 2));
    std::vector<Lindel>::iterator end;
    if (this->phase == 0) {
        end = this->lindels.end();
    } else {
        end = this->lindels.begin() + portions *
            (World::SORT_PHASES - this->phase);
    }
    std::sort(start, end, [this](Lindel const &a, Lindel const &b) {
        return a.z > b.z;
    });
    this->phase = (this->phase + 1) % (World::SORT_PHASES - 1);
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
        float distance = Util::manhattan3(this->position, lindel.position);
        if (distance >= World::SIGHT_DISTANCE) continue;
        glm::vec4 p = clipper * glm::vec4(lindel.position, 1);
        lindel.z = p.w;
        if (p.w < 0) continue;
        float scale = 2 / p.w * lindel.entity->scale;
        p = p / p.w;
        glm::vec2 screen = glm::vec2(p.x + 1, 1 - p.y) * 0.5f *
            (glm::vec2)this->size;
        renderer.batch.draw(
            lindel.entity->sprite,
            screen,
            lindel.entity->offset * scale,
            0,
            glm::vec2(scale, scale)
        );
    }
}

void World::addLindel(
    Entity const *entity,
    glm::vec3 position,
    char const *behaviourName
) {
    Behaviour const *behaviour = NULL;
    if (behaviourName && this->behaviours.count(behaviourName) > 0) {
        behaviour = &(this->behaviours.at(behaviourName));
    }
    Lindel lindel(entity, behaviour);
    lindel.position = position;
    lindel.alive = true;
    this->lindels.push_back(std::move(lindel));
}

void World::addBehaviour(char const *name, Behaviour behaviour) {
    this->behaviours[name] = behaviour;
}

Behaviour const *World::getBehaviour(char const *name) const {
    if (this->behaviours.count(name) > 0) return &(this->behaviours.at(name));
    return NULL;
}

void World::lindelBehaviour(Lindel &lindel) {
    if (!lindel.behaviour || !lindel.state) continue;
    switch (lindel.state->style) {
        case Behaviour::Style::Static:
            lindel.velocity.x = 0;
            lindel.velocity.y = 0;
            lindel.velocity.z = 0;
            break;
        case Behaviour::Style::Flap:
            // TODO: this.
        case Behaviour::Style::Jump:
            brexit();
    }
}
