/*
 * File: /tomato/tests/src/variablesTests.cpp
 * 
 * Created the 02 June 2024, 08:33 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <catch2/catch_test_macros.hpp>
#include "LuauContext.h"
#include <iostream>

TEST_CASE("Check the retrieve of the primary types variable")
{
    tomato::LuauContext script{};

    script.run("./resources/variable-primary.luau");

    std::list<std::string> allVariablesNames{ "isRunning", "distance", "pi", "libraryName" };

    try {
        std::for_each(allVariablesNames.begin(), allVariablesNames.end(), [&](const std::string &it) {
            std::cout << it << std::endl;
            if (!script.doesExist(it))
                FAIL(it + " does not exist");
            int varType = script.getVariableType(it);
            switch (varType) {
                case LUA_TBOOLEAN:
                    std::any_cast<bool>(script.getVariable(it));
                    break;
                case LUA_TNUMBER:
                    try {
                        std::any_cast<double>(script.getVariable(it));
                    } catch (const std::bad_cast &e) {
                        std::any_cast<int>(script.getVariable(it));
                    }
                    break;
                case LUA_TSTRING:
                    std::any_cast<char *>(script.getVariable(it));
                    break;

            }
        });
    } catch (const std::bad_cast &e) {
        FAIL(e.what());
    };
    /*REQUIRE_NOTHROW(std::any_cast<bool>(script.getVariable("isRunning")));
    REQUIRE(script.doesExist("distance"));
    REQUIRE_NOTHROW(std::any_cast<int>(script.getVariable("distance")));
    REQUIRE(script.doesExist("pi"));
    REQUIRE_NOTHROW(std::any_cast<double>(script.getVariable("pi")));
    REQUIRE(script.doesExist("libraryName"));
    REQUIRE_NOTHROW(std::any_cast<char *>(script.getVariable("libraryName")));
    REQUIRE(script.doesExist("optional"));
    REQUIRE_NOTHROW(std::any_cast<nullptr>(script.getVariable("optional")));*/
}