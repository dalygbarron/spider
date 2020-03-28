#include "Bullet.hh"

Bullet::Bullet(Bullet::Prototype const &prototype, Actor const &parent):
    Mob((float)prototype.rat.width / 2),
    prototype(prototype), 
    parent(parent)
{
    // Does nothing.
}
