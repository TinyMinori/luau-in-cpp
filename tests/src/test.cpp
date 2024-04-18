/*
 * File: /tomato/tests/src/test.cpp
 * 
 * Created the 06 April 2024, 05:05 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <stdexcept>
#include "test.h"

TEST_CASE("Testing Tomato")
{
    tomato::LuauContext script;
    script.run("resources/existing-variables.luau");

    REQUIRE(script.doesExist("result") == true);

    // Sections would actually run the code from the beginning of the test case
    // but they you will run sections one by one
    SECTION("A Section")
    {
        REQUIRE(script.doesExist("i") == false);
    }
}

TEST_CASE("Run scripts in tomato")
{
    tomato::LuauContext script;
    SECTION("Check script opening") {
        CHECK_THROWS_MATCHES(
            script.run("resources/non-existing.luau"),
            std::runtime_error,
            Catch::Matchers::ExceptionMessageMatcher("The script named resources/non-existing.luau is not a correct script location."));
        
        CHECK_NOTHROW(script.run("resources/existing-script.luau"));
    }
}