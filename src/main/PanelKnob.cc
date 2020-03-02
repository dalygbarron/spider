#include "Knob.hh"
#include "Renderer.hh"
#include <cmath>

PanelKnob::PanelKnob(int x, int y, int w, int h, int parts):
    Knob(x, y, w, h, -1),
    parts(parts)
{
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

void PanelKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    renderer.panel(this->shape);
    for (Knob *child: this->children) child->draw(target, renderer);
}
