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
    if (measurements.normalFontSize.x == 0 ||
        measurements.normalFontSize.y == 0
    ) {
        return;
    }
    int columns = w / fmax(measurements.normalFontSize.x - 1, 1);
    int rows = h / measurements.normalFontSize.y;
    if (columns == 0) columns = 1;
    this->text = text;
    int n = this->text.size();
    int start = 0;
    while (start < n) {
        int end = start + columns;
        if (end > n - 1) {
            end = n - 1;
        } else {
            while (this->text[end] != ' ' && end > start) end--;
            if (end == start) end = fmin(start + columns, n - 1);
        }
        if (end != n - 1) this->text[end] = '\n';
        start = end + 1;
    }
}

void TextKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    sf::Vector2f pos(this->shape.left, this->shape.top);
    renderer.text(this->text, pos);
}
