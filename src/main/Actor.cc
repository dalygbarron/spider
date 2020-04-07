#include "Actor.hh"

Actor::Actor(Rat rat, float radius):
    Mob(radius)
{
    this->rat = rat;
}

void Actor::update() {
    this->rat.update();
}
