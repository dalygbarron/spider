#include "Knob.hh"
#include "Renderer.hh"
#include "spdlog/spdlog.h"

FrameKnob::FrameKnob(int x, int y, int w, int h, Rectangle rat):
    Knob(x, y, w, h, -1),
    rat(rat)
{
    float scale = fmin((float)w / rat.size.x, (float)h / rat.size.y);
    this->drawPos.pos.x = x;
    this->drawPos.pos.y = y;
    this->drawPos.size = (glm::vec2)rat.size * scale;
}

void FrameKnob::draw(sf::RenderTarget &target, Renderer &renderer) const {
    renderer.batch.draw(rat, this->drawPos);
}
