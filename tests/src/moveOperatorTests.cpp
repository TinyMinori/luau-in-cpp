/*
 * File: /tomato/tests/src/moveOperatorTests.cpp
 * 
 * Created the 19 May 2024, 09:24 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include "LuauContext.h"

TEST_CASE("Check move constructor and operator")
{
    tomato::LuauContext firstScript{};
    tomato::LuauContext moveConstr(std::move(firstScript));
    tomato::LuauContext moveOpp{};
    moveOpp = std::move(moveConstr);

    REQUIRE(firstScript.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    REQUIRE(moveConstr.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    REQUIRE(moveOpp.run("./resources/existing-script.luau") == LUA_OK);
}