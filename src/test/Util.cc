#include "catch.hpp"
#include "Help.hh"
#include "../main/Util.hh"
#include "../main/Const.hh"
#include "../main/spdlog/spdlog.h"

TEST_CASE("slice checks", "[maths]") {
    REQUIRE(Util::inSlice(1, 2, 1.5));
    REQUIRE(Util::inSlice(5, 1, 6));
    REQUIRE(!Util::inSlice(1, 2, 3));
    REQUIRE(!Util::inSlice(6, 0.2, 4));
}

TEST_CASE("inverse rotation matrices", "[maths]") {
    sf::Vector3f rotation(1, 2, 3);
    Matrix a = Util::rotationMatrix(rotation);
    Matrix aI = Util::inverseRotationMatrix(rotation);
    a = a.combine(aI);
    REQUIRE(a.content[0][0] == Approx(1).margin(0.01));
    REQUIRE(a.content[0][1] == Approx(0).margin(0.01));
    REQUIRE(a.content[0][2] == Approx(0).margin(0.01));
    REQUIRE(a.content[0][3] == Approx(0).margin(0.01));
    REQUIRE(a.content[1][0] == Approx(0).margin(0.01));
    REQUIRE(a.content[1][1] == Approx(1).margin(0.01));
    REQUIRE(a.content[1][2] == Approx(0).margin(0.01));
    REQUIRE(a.content[1][3] == Approx(0).margin(0.01));
    REQUIRE(a.content[2][0] == Approx(0).margin(0.01));
    REQUIRE(a.content[2][1] == Approx(0).margin(0.01));
    REQUIRE(a.content[2][2] == Approx(1).margin(0.01));
    REQUIRE(a.content[2][3] == Approx(0).margin(0.01));
    REQUIRE(a.content[3][0] == Approx(0).margin(0.01));
    REQUIRE(a.content[3][1] == Approx(0).margin(0.01));
    REQUIRE(a.content[3][2] == Approx(0).margin(0.01));
    REQUIRE(a.content[3][3] == Approx(1).margin(0.01));
}

TEST_CASE("cartesian and spherical coordinates", "[maths]") {
    sf::Vector3f cartesian(1, 1, -1);
    sf::Vector2f spherical = Util::cartesianToSpherical(cartesian);
    sf::Vector3f back = Util::sphericalToCartesian(spherical);
    REQUIRE(spherical.x == 0.707);
    REQUIRE(spherical.y == 0.707);
    REQUIRE(cartesian.x == back.x);
    REQUIRE(cartesian.y == back.y);
    REQUIRE(cartesian.z == back.z);
}
