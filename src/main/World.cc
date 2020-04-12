#include "World.hh"

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

void World::draw(sf::RenderTarget &target, sf::Vector2f camera) const {
    // Draw the ground and sky.
    // Draw the fishes.
}
