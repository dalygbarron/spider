#include "Knob.hh"
#include "Renderer.hh"
#include "spdlog/spdlog.h"
#include <cmath>

TextKnob::TextKnob(
    int x,
    int y,
    int w,
    int h,
    Rectangle font,
    char const *text
):
    Knob(x, y, w, h, -1)
{
    this->font = font;
    if (font.size.x == 0 || font.size.y == 0) return;
    glm::ivec2 character = font.size / 16;
    int columns = w / fmax(character.x - 1, 1);
    int rows = h / character.y;
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
    renderer.text(this->text.c_str(), this->shape.pos, this->font);
}
