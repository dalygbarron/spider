#include "Knob.hh"
#include <cmath>

PanelKnob::PanelKnob(int parts): parts(parts) {
    // Nothing else.
}

void PanelKnob::addChild(Knob *child) {
    this->children.push_back(child);
}

void PanelKnob::bake(sf::FloatRect shape) {
    Knob::bake(shape);
    int rows = ceil((float)this->children.size() / this->parts);
    shape.left += PanelKnob::BORDER;
    shape.top += PanelKnob::BORDER;
    shape.width = (shape.width -  PanelKnob::BORDER * 2) / this->parts;
    shape.height = (shape.height - PanelKnob::BORDER * 2) / rows;
    for (int i = 0; i < this->children.size(); i++) {
        this->children[i]->bake(sf::FloatRect(
            shape.left + (i % this->parts) * shape.width,
            shape.top + ceil(i / this->parts + 1) * shape.height,
            shape.width,
            shape.height
        ));
    }
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
    renderer.box(this->getShape(), false);
    for (Knob *child: this->children) child->draw(renderer);
}
