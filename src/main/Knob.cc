#include "Knob.hh"

Knob::~Knob() {
    // Does nothing.
}

int Knob::isBaked() {
    return this->baked;
}

void bake(sf::FloatRect shape) {
    this->shape = shape;
}

Knob *Knob::update(sf::Vector2f mouse, SoundPlayer &soundPlayer) {
    return NULL;
}
