#include "Bullet.hh"

void Bullet::update(float delta) {
    this->position.x += this->velocity.x * delta;
    this->position.y += this->velocity.y * delta;
    this->velocity.x += this->gravity.x * delta;
    this->velocity.y += this->gravity.y * delta;
}
