#include "catch.hpp"
#include "Help.hh"
#include "../main/Util.hh"
#include "../main/Const.hh"
#include "../main/spdlog/spdlog.h"

TEST_CASE("what does sprintf do", "[shit]") {
    char string[100];
    int number;
    sscanf("tangoIsAMoron 600", "%s %d", string, &number);
    REQUIRE_THAT(string, Catch::Equals("tangoIsAMoron"));
    REQUIRE(number == 600);
}
