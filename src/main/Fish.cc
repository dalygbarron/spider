#include "Fish.hh"

void Fish::update() {
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
}
