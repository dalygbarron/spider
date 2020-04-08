#include "Actor.hh"

Actor::Actor(float radius, Rat rat):
    Mob(radius),
    rat(rat)
{
    // does nothing else.
}

void Actor::actorUpdate() {
    this->rat.update();
    if (this->walkAnimation) {
        if (this->velocity.x != 0 || this->velocity.y != 0) {
            this->rat.play(this->walkAnimation, 0);
        } else if (this->rat.getCurrent() == this->walkAnimation) {
            this->rat.stop();
        }
    }
}
