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
#include <stdexcept>
#include "Context.h"
#include "Reader.h"
#include "Exceptions.h"

TEST_CASE("Check existing variables")
{
    tomato::LuauContext script = tomato::LuauReader::getContextFromFile("./resources/existing-variables.luau");

    REQUIRE(script.doesExist("globalVar") == true);
    REQUIRE(script.doesExist("localScopeVar") == false);
    REQUIRE(script.doesExist("doesNotExist") == false);
}

TEST_CASE("Check script opening")
{
    try {
        tomato::LuauReader::getContextFromFile("./resources/non-existing.luau");
        FAIL("Must failed but succeed");
    } catch (const tomato::LuauException &e) {
        if (e.getId() == tomato::LuauExceptions::FileDoesNotExist)
            SUCCEED("Failed successfully");
        else
            FAIL("Failed with the wrong exception");
    }

    CHECK_NOTHROW(tomato::LuauReader::getContextFromFile("./resources/existing-script.luau"));
}