#include "Limiter.hh"

Limiter::Limiter(float rate):
    rate(rate),
    interval(1 / rate)
{
    // all done.
}

void Limiter::update(float delta, std::function<void(float)> const &action) {
    time += delta;
    while (i < time / interval) {
        action(interval);
        i++;
    }
}
