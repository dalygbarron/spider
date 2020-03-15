#include "Knob.hh"
#include "Renderer.hh"
#include "spdlog/spdlog.h"

FrameKnob::FrameKnob(int x, int y, int w, int h, sf::IntRect rat):
    Knob(x, y, w, h, -1),
    rat(rat)
{
    float scale = fmin(
        (float)w / rat.width,
        (float)h / rat.height
    );
    spdlog::info("{}", scale);
    this->drawPos.left = x;
    this->drawPos.top = y;
    this->drawPos.width = rat.width * scale;
    this->drawPos.height = rat.height * scale;
}

void FrameKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    renderer.batch.draw(rat, this->drawPos);
}
