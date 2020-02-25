#include "Knob.hh"

Knob::~Knob() {
    // Does nothing.
}

int Knob::isBaked() const {
    return this->baked;
}

sf::FloatRect Knob::getShape() const {
    return this->shape;
}

void Knob::bake(sf::FloatRect shape) {
    this->shape = shape;
    this->baked = true;
}

Knob *Knob::update(sf::Vector2f mouse, SoundPlayer &soundPlayer) {
    return NULL;
}
