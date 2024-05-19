/*
 * File: /tomato/tests/src/test.cpp
 * 
 * Created the 25 April 2024, 10:26 pm by TinyMinori
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
#include "LuauContext.h"

TEST_CASE("Check existing variables")
{
    tomato::LuauContext script{};
    script.run("./resources/existing-variables.luau");

    REQUIRE(script.doesExist("result") == true);
    REQUIRE(script.doesExist("i") == false);
}

TEST_CASE("Check script opening")
{
    tomato::LuauContext script{};
    CHECK_THROWS_MATCHES(
        script.run("./resources/non-existing.luau"),
        std::runtime_error,
        Catch::Matchers::ExceptionMessageMatcher("The script named ./resources/non-existing.luau is not a correct script location."));
    
    CHECK_NOTHROW(script.run("./resources/existing-script.luau"));
}