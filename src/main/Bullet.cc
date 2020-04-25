#include "Bullet.hh"

Bullet::Bullet(Bullet::Prototype const *prototype, unsigned int parent):
    Mob((float)prototype->rat.size.x / 2),
    prototype(prototype)
{
    this->parent = parent;
}
