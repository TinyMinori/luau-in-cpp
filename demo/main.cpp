/*
 * File: /tomato/demo/main.cpp
 * 
 * Created the 04 May 2023, 11:31 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2023 TinyMinori
 */

#include "LuauScript.h"
#include <iostream>

void    help_usage(tomato::fs::path programName) {
    std::cout << "Usage :" << std::endl;
    std::cout << "./" << programName.filename().u8string() << " [luau_script_filepath]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        help_usage(argv[0]);  
        return EXIT_FAILURE;
    }
    tomato::LuauScript  script {};

    std::cout << std::boolalpha;
    script.run(argv[1]);
    auto result = script.runFunction("iseven", {2}, 1);
    std::cout << "[C] Is 2 even: " << std::any_cast<bool>(result.front()) << std::endl << std::endl;

    result = script.runFunction("iseven", {3}, 1);
    std::cout << "[C] Is 3 even: " << std::any_cast<bool>(result.front()) << std::endl << std::endl;

    result = script.runFunction("returnFunc", {}, 1);
    std::cout << "[C] string of returnFunc: " << std::any_cast<char *>(result.front()) << std::endl << std::endl;

    result = script.runFunction("hello", {}, 0);
    std::cout << "[C] is hello function return empty : " << result.empty() << std::endl << std::endl;

    result = script.runFunction("zozo", {}, 0);
    std::cout << std::endl;
    std::cout << "does var `i` exist: " << script.doesExist("i") << std::endl;
    std::cout << "does function `iseven` exist: " << script.doesExist("iseven") << std::endl;
    std::cout << "does var `result` exist: " << script.doesExist("result") << std::endl;
}