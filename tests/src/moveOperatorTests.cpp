/*
 * File: /tomato/tests/src/moveOperatorTests.cpp
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include "LuauState.h"
#include "LuauReader.h"

TEST_CASE("Check move constructor and operator")
{
    LuauState firstScript = LuauReader::getContextFromFile("./resources/existing-script.luau");
    LuauState moveConstr(std::move(firstScript));
    LuauState moveOpp = LuauReader::getContextFromFile("./resources/existing-script.luau");
    moveOpp = std::move(moveConstr);

    //REQUIRE(firstScript.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    //REQUIRE(moveConstr.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    //REQUIRE(moveOpp.run("./resources/existing-script.luau") == LUA_OK);
}