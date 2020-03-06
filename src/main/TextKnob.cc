#include "Knob.hh"
#include "Renderer.hh"
#include "spdlog/spdlog.h"
#include <cmath>

TextKnob::TextKnob(
    int x,
    int y,
    int w,
    int h,
    Measurements const &measurements,
    char const *text
):
    Knob(x, y, w, h, -1)
{
    int columns = w / measurements.normalFontSize.x;
    int rows = h / measurements.normalFontSize.y;
    this->text = text;
    int n = this->text.size();
    int start = 0;
    while (start < n) {
        int end = start + columns;
        if (end > n - 1) {
            end = n - 1;
        } else {
            while (this->text[end] != ' ' && end > start) end--;
            if (end == start) end = fmin(columns, n - 1);
        }
        if (end != n - 1) this->text[end] = '\n';
        start = end + 1;
    }
}

void TextKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    sf::Vector2f pos(this->shape.left, this->shape.top);
    renderer.text(this->text, pos);
}
