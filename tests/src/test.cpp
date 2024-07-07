/*
 * File: /tomato/tests/src/test.cpp
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include "LuauState.h"
#include "LuauReader.h"
#include "LuauException.h"

TEST_CASE("Check existing variables")
{
    LuauState script = LuauReader::getContextFromFile("./resources/existing-variables.luau");

    REQUIRE(script.getGlobals().doesExist("globalVar") == true);
    REQUIRE(script.getGlobals().doesExist("localScopeVar") == false);
    REQUIRE(script.getGlobals().doesExist("doesNotExist") == false);
}

TEST_CASE("Check script opening")
{
    try {
        LuauReader::getContextFromFile("./resources/non-existing.luau");
        FAIL("Must failed but succeed");
    } catch (const LuauException &e) {
        if (e.getId() == FileDoesNotExist)
            SUCCEED("Failed successfully");
        else
            FAIL("Failed with the wrong exception");
    }

    CHECK_NOTHROW(LuauReader::getContextFromFile("./resources/existing-script.luau"));
}