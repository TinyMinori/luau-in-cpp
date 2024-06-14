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
#include "Context.h"
#include "Reader.h"

TEST_CASE("Check move constructor and operator")
{
    tomato::LuauContext firstScript = tomato::LuauReader::getContextFromFile("./resources/existing-script.luau");
    tomato::LuauContext moveConstr(std::move(firstScript));
    tomato::LuauContext moveOpp = tomato::LuauReader::getContextFromFile("./resources/existing-script.luau");
    moveOpp = std::move(moveConstr);

    //REQUIRE(firstScript.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    //REQUIRE(moveConstr.run("./resources/existing-script.luau") == STATE_NOT_INIALIZED);
    //REQUIRE(moveOpp.run("./resources/existing-script.luau") == LUA_OK);
}