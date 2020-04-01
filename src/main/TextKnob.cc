#include "Knob.hh"
#include "Renderer.hh"
#include "spdlog/spdlog.h"
#include <cmath>

TextKnob::TextKnob(
    int x,
    int y,
    int w,
    int h,
    sf::IntRect font,
    char const *text
):
    Knob(x, y, w, h, -1)
{
    this->font = font;
    if (font.width == 0 || font.height == 0) return;
    float characterWidth = font.width / 16;
    float characterHeight = font.height / 16;
    int columns = w / fmax(characterWidth - 1, 1);
    int rows = h / characterHeight;
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
    renderer.text(this->text.c_str(), pos, this->font);
}
