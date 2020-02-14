#include "catch.hpp"
#include "Help.hh"
#include "../main/Util.hh"
#include "../main/Const.hh"
#include "../main/spdlog/spdlog.h"

TEST_CASE("clean rotations", "[maths]") {
    sf::Vector2f front(0, 0);
    sf::Vector2f right = Util::rotate(front, sf::Vector2f(Const::HALF_PI, 0));
    sf::Vector2f left = Util::rotate(front, sf::Vector2f(-Const::HALF_PI, 0));
    sf::Vector2f back = Util::rotate(front, sf::Vector2f(Const::PI, 0));
    sf::Vector2f bottom = Util::rotate(front, sf::Vector2f(0, Const::HALF_PI));
    sf::Vector2f top = Util::rotate(front, sf::Vector2f(0, -Const::HALF_PI));
    sf::Vector2f rightAgain = Util::rotate(top, sf::Vector2f(Const::HALF_PI, Const::HALF_PI));
    sf::Vector2f leftAgain = Util::rotate(bottom, sf::Vector2f(-Const::HALF_PI, -Const::HALF_PI));
    sf::Vector2f topAgain = Util::rotate(bottom, sf::Vector2f(0, -Const::PI));
    Help::compareSphere(right, sf::Vector2f(Const::HALF_PI, 0));
    Help::compareSphere(rightAgain, sf::Vector2f(Const::HALF_PI, 0));
    Help::compareSphere(left, sf::Vector2f(-Const::HALF_PI, 0));
    Help::compareSphere(leftAgain, sf::Vector2f(-Const::HALF_PI, 0));
    Help::compareSphere(back, sf::Vector2f(Const::PI, 0));
    Help::compareSphere(bottom, sf::Vector2f(0, Const::HALF_PI));
    Help::compareSphere(top, sf::Vector2f(0, -Const::HALF_PI));
    Help::compareSphere(topAgain, sf::Vector2f(0, -Const::HALF_PI));
}

TEST_CASE("slice checks", "[maths]") {
    REQUIRE(Util::inSlice(1, 2, 1.5));
    REQUIRE(Util::inSlice(5, 1, 6));
    REQUIRE(!Util::inSlice(1, 2, 3));
    REQUIRE(!Util::inSlice(6, 0.2, 4));
}
