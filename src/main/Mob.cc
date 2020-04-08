#include "Mob.hh"
#include <cmath>

Mob::Mob(float radius): radius(radius)
{
    // nothing else.
}

void Mob::update() {
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
    this->velocity.x += this->gravity.x;
    this->velocity.y += this->gravity.y;
}

int Mob::collide(Mob const &a, Mob const &b) {
    float join = a.radius + b.radius;
    float dX = fabs(a.position.x - b.position.x);
    if (dX > join) return false;
    float dY = fabs(a.position.y - b.position.y);
    if (dY > join) return false;
    return (sqrt(dX * dX + dY * dY) < fmax(join - 1, 1));
}
