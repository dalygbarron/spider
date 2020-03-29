#include "Actor.hh"

Actor::Actor(sf::IntRect rat, float radius):
    Mob(radius)
{
    this->rat = rat;
}
