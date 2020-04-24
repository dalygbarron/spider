#include "Mob.hh"
#include <cmath>

Mob::Mob(float radius): radius(radius)
{
    // nothing else.
}

voidsf::Vector2f Mob::update() {
    this->position += this->velocity;
    this->velocity += this->gravity;
}

int Mob::collide(Mob const &a, Mob const &b) {
    float join = a.radius + b.radius;
    float dX = fabs(a.position.x - b.position.x);
    if (dX > join) return false;
    float dY = fabs(a.position.y - b.position.y);
    if (dY > join) return false;
    return (sqrt(dX * dX + dY * dY) < fmax(join - 1, 1));
}
