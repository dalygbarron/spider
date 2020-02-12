#include "Help.hh"
#include "catch.hpp"
#include "../main/spdlog/spdlog.h"

void Help::close(float a, float b) {
    REQUIRE(abs(a - b) < 0.0000001);
}

void Help::compareVector2f(sf::Vector2f a, sf::Vector2f b) {
    Help::close(a.x, b.x);
    Help::close(a.y, b.y);
}

void Help::compareVector3f(sf::Vector3f a, sf::Vector3f b) {
    Help::close(a.x, b.x);
    Help::close(a.y, b.y);
    Help::close(a.z, b.z);
}
