#include "Knob.hh"

ButtonKnob::ButtonKnob(Knob *child) {
    this->child = child;
}

ButtonKnob::~ButtonKnob() {
    if (this->child) delete this->child;
}

void ButtonKnob::bake(sf::FloatRect shape) {
    Knob::bake(shape);
    shape.left += ButtonKnob::BORDER;
    shape.top += ButtonKnob::BORDER;
    shape.width -= ButtonKnob::BORDER * 2;
    shape.height -= ButtonKnob::BORDER * 2;
    if (this->child) this->child->bake(shape);
}

Knob *ButtonKnob::update(sf::Vector2f mouse, SoundPlayer &soundPlayer) {
    if (!this->enabled) return NULL;
    int pastDepression = this->depressed;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->depressed = this->getShape().contains(mouse);
    } else if (this->depressed) {
        this->depressed = false;
        // TODO: sound.play("buttonUp");
        if (this->getShape().contains(mouse)) return this;
    }
    if (this->depressed && !pastDepression) {
        // TODO: sound.play("buttonDown");
    }
    return NULL;
}

void ButtonKnob::draw(Renderer &renderer) const {
    renderer.button(this->getShape(), !this->enabled || this->depressed);
    if (this->child) this->child->draw(renderer);
}


