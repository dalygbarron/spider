#include "Knob.hh"

Knob::Knob(int x, int y, int w, int h, int id):
    shape(x, y, w, h),
    id(id)
{
    // nothing else to be done.
}

Knob::~Knob() {
    // Does nothing.
}

Knob *Knob::update(glm::ivec2 mouse, SoundPlayer &soundPlayer) {
    return NULL;
}
