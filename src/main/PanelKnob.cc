#include "Knob.hh"
#include "Renderer.hh"
#include <cmath>

PanelKnob::PanelKnob(int parts): parts(parts) {
    // Nothing else.
}

PanelKnob::~PanelKnob() {
    for (Knob *child: this->children) delete child;
}

void PanelKnob::addChild(Knob *child) {
    this->children.push_back(child);
}

Knob *PanelKnob::update(
    sf::Vector2f mouse,
    SoundPlayer &soundPlayer
) {
    for (Knob *child: this->children) {
        Knob *response = child->update(mouse, soundPlayer);
        if (response) return response;
    }
    return NULL;
}

void PanelKnob::draw(Renderer &renderer) const {
    renderer.panel(this->shape);
    for (Knob *child: this->children) child->draw(renderer);
}
