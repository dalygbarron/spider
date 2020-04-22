#include "catch.hpp"
#include "Help.hh"
#include "../main/Memory.hh"

TEST_CASE("Switch expression XOR test", "[memory][switch]") {
    Memory memory(1);
    Memory::SwitchExpression *a = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::Switch,
        "a",
        NULL,
        NULL
    );
    Memory::SwitchExpression *b = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::Switch,
        "b",
        NULL,
        NULL
    );
    Memory::SwitchExpression *andExpression = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::And,
        NULL,
        a,
        b
    );
    Memory::SwitchExpression *orExpression = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::Or,
        NULL,
        a,
        b
    );
    Memory::SwitchExpression *nand = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::Not,
        NULL,
        andExpression,
        NULL
    );
    Memory::SwitchExpression *finalAnd = new Memory::SwitchExpression(
        Memory::SwitchExpression::Type::And,
        NULL,
        nand,
        orExpression
    );
    // both inputs off.
    REQUIRE(!finalAnd->evaluate(memory));
    // both inputs on.
    memory.setSwitch("a", true);
    memory.setSwitch("b", true);
    REQUIRE(!finalAnd->evaluate(memory));
    // a on b off.
    memory.setSwitch("b", false);
    REQUIRE(finalAnd->evaluate(memory));
    // a off b on.
    memory.setSwitch("a", false);
    memory.setSwitch("b", true);
    REQUIRE(finalAnd->evaluate(memory));
}
