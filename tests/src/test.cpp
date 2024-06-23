/*
 * File: /tomato/tests/src/test.cpp
 * 
 * Created the 02 June 2024, 08:33 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <stdexcept>
#include "LuauState.h"
#include "LuauReader.h"
#include "LuauExceptions.h"

TEST_CASE("Check existing variables")
{
    LuauState script = LuauReader::getContextFromFile("./resources/existing-variables.luau");

    REQUIRE(script.doesExist("globalVar") == true);
    REQUIRE(script.doesExist("localScopeVar") == false);
    REQUIRE(script.doesExist("doesNotExist") == false);
}

TEST_CASE("Check script opening")
{
    try {
        LuauReader::getContextFromFile("./resources/non-existing.luau");
        FAIL("Must failed but succeed");
    } catch (const Exception &e) {
        if (e.getId() == LuauExceptions::FileDoesNotExist)
            SUCCEED("Failed successfully");
        else
            FAIL("Failed with the wrong exception");
    }

    CHECK_NOTHROW(LuauReader::getContextFromFile("./resources/existing-script.luau"));
}