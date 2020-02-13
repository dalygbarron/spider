#include "Help.hh"
#include "catch.hpp"
#include "../main/spdlog/spdlog.h"
#include "../main/Const.hh"

void Help::close(float a, float b) {
    REQUIRE(abs(a - b) < 0.0000001);
}

void Help::compareVector2f(sf::Vector2f a, sf::Vector2f b) {
    Help::close(a.x, b.x);
    Help::close(a.y, b.y);
}

void Help::compareSphere(sf::Vector2f a, sf::Vector2f b) {
    float dX = fmod(abs(a.x - b.x), Const::DOUBLE_PI);
    float dY = fmod(abs(a.y - b.y), Const::DOUBLE_PI);
    REQUIRE(dY < 0.000001);
    if (a.y != Approx(Const::HALF_PI) && a.y != Approx(-Const::HALF_PI)) {
        REQUIRE(dX == Approx(0));
    }
}
