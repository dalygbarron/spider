#include "Bullet.hh"

Bullet::Prototype Prototype(sf::IntRect rat, float radius):
    rat(rat),
    radius(radius)
{
    // does nothign.
}

Bullet::Bullet(Bullet::Prototype const &prototype, Actor const &parent):
    Mob((float)prototype.rat.width / 2),
    prototype(prototype), 
    parent(parent)
{
    // Does nothing.
}
