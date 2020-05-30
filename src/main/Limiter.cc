#include "Limiter.hh"
#include <cmath>

Limiter::Limiter(float rate):
    rate(rate),
    interval(1.0f / rate)
{
    // all done.
}

int Limiter::update(float delta) {
    this->time += delta;
    int oldI = this->i;
    this->i = floor(this->time / this->interval);
    return this->i - oldI;
}
