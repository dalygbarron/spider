#include "catch.hpp"
#include "Help.hh"
#include "../main/Util.hh"
#include "../main/Const.hh"
#include "../main/spdlog/spdlog.h"

TEST_CASE("clean rotations", "[maths]") {
    sf::Vector3f front(0, 0, 1);
    sf::Vector3f right = Util::rotate(front, sf::Vector2f(Const::HALF_PI, 0));
    sf::Vector3f left = Util::rotate(front, sf::Vector2f(-Const::HALF_PI, 0));
    sf::Vector3f back = Util::rotate(front, sf::Vector2f(Const::PI, 0));
    sf::Vector3f bottom = Util::rotate(front, sf::Vector2f(0, Const::HALF_PI));
    sf::Vector3f top = Util::rotate(front, sf::Vector2f(0, -Const::HALF_PI));
    sf::Vector3f rightAgain = Util::rotate(right, sf::Vector2f(0, Const::HALF_PI));
    sf::Vector3f leftAgain = Util::rotate(bottom, sf::Vector2f(Const::HALF_PI, Const::HALF_PI));
    sf::Vector3f topAgain = Util::rotate(bottom, sf::Vector2f(0, Const::PI));
    Help::compareVector3f(right, sf::Vector3f(1, 0, 0));
    Help::compareVector3f(rightAgain, sf::Vector3f(1, 0, 0));
    Help::compareVector3f(left, sf::Vector3f(-1, 0, 0));
    Help::compareVector3f(leftAgain, sf::Vector3f(-1, 0, 0));
    Help::compareVector3f(back, sf::Vector3f(0, 0, -1));
    Help::compareVector3f(bottom, sf::Vector3f(0, -1, 0));
    Help::compareVector3f(top, sf::Vector3f(0, 1, 0));
    Help::compareVector3f(topAgain, sf::Vector3f(0, 1, 0));
}

TEST_CASE("sphere to point and point to sphere", "[maths]") {

}
