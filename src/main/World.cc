#include "World.hh"
#include "Const.hh"

std::pair<char const *, char const *> World::update(sf::Vector2f camera) {
    char const *function = NULL;
    char const *argument = NULL;
    // Update the fishes.
    for (Fish const &fish: this->fishes) {
        // TODO: update the fish.
    }
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
    sf::Vector2f camera
) const {
    // Draw the ground and sky.
    sf::RenderStates states;
    states.shader = &(this->shader);
    target.draw(back, states);
    // Draw the fishes.
    sf::Vector3f floor = Util::sphereToScreen(
        sf::Vector2f(0, Const::HALF_PI * ((camera.y > 0) ? 1 : -1)),
        camera
    );
    sf::Vector2f floorScreen(floor.x, floor.y);
    for (Fish const &fish: this->fishes) {
        if (!fish.alive) continue;
        sf::Vector3f spherePos = Util::toSphere(fish.position, this->position);
        sf::Vector3f screenPos = Util::sphereToScreen(sf::Vector2f(
            spherePos.x,
            spherePos.y
        ), camera);
        if (screenPos.z < 0) continue;
        float angle = Util::upAngle(
            camera,
            floorScreen,
            sf::Vector2f(screenPos.x, screenPos.y)
        );
        float scale = 1 / screenPos.z;
        renderer.batch.draw(
            fish.entity->sprite,
            sf::Vector2f(screenPos.x, screenPos.y),
            fish.entity->offset,
            angle,
            sf::Vector2f(scale, scale)
        );
    }
}
