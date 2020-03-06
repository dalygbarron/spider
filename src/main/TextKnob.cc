#include "Knob.hh"
#include "Renderer.hh"

TextKnob::TextKnob(int x, int y, int w, int h, char const *text):
    Knob(x, y, w, h, -1),
    text(text)
{
}

void TextKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    sf::Vector2f pos(this->shape.left, this->shape.top);
    renderer.text(this->text, pos);
}
