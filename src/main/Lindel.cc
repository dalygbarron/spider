#include "Lindel.hh"

Lindel::Lindel(Entity const *entity, Behaviour const *behaviour):
    entity(entity),
    position(0),
    velocity(0),
    gravity(0)
{
    this->behaviour = behaviour;
    if (this->behaviour) this->state = behaviour->start;
}

void Lindel::update() {
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->position.z += this->velocity.z;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
    this->velocity.z += this->gravity.z;
}
