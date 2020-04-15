#include "Lindel.hh"

Lindel::Lindel(Entity const &entity): entity(entity) {
    // nothing else.
}

void Lindel::update() {
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
}
