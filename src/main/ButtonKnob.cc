#include "Knob.hh"
#include "Renderer.hh"

ButtonKnob::ButtonKnob(int x, int y, int w, int h, int id, Knob *child):
    Knob(x, y, w, h, id)
{
    this->child = child;
}

ButtonKnob::~ButtonKnob() {
    if (this->child) delete this->child;
}

Knob *ButtonKnob::update(sf::Vector2f mouse, SoundPlayer &soundPlayer) {
    if (!this->enabled) return NULL;
    int pastDepression = this->depressed;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->depressed = this->shape.contains(mouse);
    } else if (this->depressed) {
        this->depressed = false;
        // TODO: sound.play("buttonUp");
        if (this->shape.contains(mouse)) return this;
    }
    if (this->depressed && !pastDepression) {
        // TODO: sound.play("buttonDown");
    }
    return NULL;
}

void ButtonKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    renderer.button(this->shape, !this->enabled || this->depressed);
    if (this->child) this->child->draw(target, renderer);
}


