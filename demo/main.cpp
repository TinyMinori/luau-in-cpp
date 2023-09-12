/*
 * File: /tomato/demo/main.cpp
 * 
 * Created the 04 May 2023, 11:31 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2023 TinyMinori
 */

#include "LuauContext.h"
#include <iostream>

void    help_usage(tomato::fs::path programName) {
    std::cout << "Usage :" << std::endl;
    std::cout << "./" << programName.filename().c_str() << " [luau_script_filepath]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        help_usage(argv[0]);  
        return EXIT_FAILURE;
    }
    tomato::LuauContext  script {};

    std::cout << std::boolalpha;
    script.run(argv[1]);
    auto result = script.runFunction("iseven", {2}, 1);
    std::cout << "[C] Is 2 even: " << script.pop<bool>(-1) << std::endl << std::endl;

    result = script.runFunction("iseven", {3}, 1);
    script.dumpstack();
    std::cout << "[C] Is 3 even: " << script.pop<bool>(-1) << std::endl << std::endl;

    result = script.runFunction("returnFunc", {}, 1);
    std::cout << "[C] string of returnFunc: " << script.pop<const char *>(-1) << std::endl << std::endl;

    script.dumpstack();
    result = script.runFunction("hello", {}, 0);
    std::cout << "[C] is hello function return empty : " << result.empty() << std::endl << std::endl;

    result = script.runFunction("zozo", {}, 0);
    std::cout << std::endl;
    std::cout << "does var `i` exist: " << script.doesExist("i") << std::endl;
    std::cout << "does function `iseven` exist: " << script.doesExist("iseven") << std::endl;
    std::cout << "does var `result` exist: " << script.doesExist("result") << std::endl;
}