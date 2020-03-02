#include "Knob.hh"

TextKnob::TextKnob(int x, int y, int w, int h, char const *text):
    Knob(x, y, w, h, -1)
{
    std::string content = text;
    this->text.setString(content);
}

void TextKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    target.draw(this->text);
}
