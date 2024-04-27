/*
 * File: /tomato/demo/src/main.cpp
 * 
 * Created the 04 May 2023, 11:31 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauContext.h"
#include <iostream>
#include <cmath>
#include <algorithm>

bool is_number(const std::string &s) {
    std::string inner = s;
    
    if (inner.empty())
        return false;
    
    if (inner.at(0) == '-')
        inner.erase(0, 1);

    if (inner.empty())
        return false;

    return std::find_if(
            inner.begin(),
            inner.end(),
            [](unsigned char c) { return !std::isdigit(c); }
        ) == inner.end();
}


void    help_usage(tomato::fs::path programName) {
    std::cout << "Usage :" << std::endl;
    std::cout << "./" << programName.filename().c_str() << " [luau_script_filepath]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        help_usage(argv[0]);  
        return EXIT_FAILURE;
    }
    tomato::LuauContext  script = tomato::LuauContext();
    script = tomato::LuauContext();

    std::cout << std::boolalpha;
    script.run(argv[1]);

    auto result = script.runFunction("isHere", {});
    std::cout << "[C] Is 2 even: " << std::any_cast<bool>(result.front()) << std::endl << std::endl;

    std::string input;
    int         iptNum;
    while (true) {
        std::cout << "Input a number: ";
        std::cin >> input;

        if (input == "exit" || input == "q" || input == "quit") {
            std::cout << std::endl;
            break;
        }

        if (!is_number(input))
            continue;

        iptNum = stoi(input);
        result = script.runFunction("isPositive", {iptNum});
        std::cout << "[C] Is " << iptNum << " positive: " << std::any_cast<bool>(result.front()) << std::endl << std::endl;

    }

    result = script.runFunction("isEven", {3});
    std::cout << "[C] Is 3 even: " << std::any_cast<bool>(result.front())  << std::endl << std::endl;

    result = script.runFunction("returnFunc", {});
    std::cout << "[C] string of returnFunc: " << std::any_cast<char *>(result.front()) << std::endl << std::endl;
    
    result = script.runFunction("printHello", {});
    std::cout << "[C] is hello function return empty : " << result.empty() << std::endl << std::endl;

    result = script.runFunction("zozo", {});

    std::cout << std::endl;
    std::cout << "does var `i` exist: " << script.doesExist("i") << std::endl;
    std::cout << "does function `iseven` exist: " << script.doesExist("isEven") << std::endl;
    std::cout << "does var `result` exist: " << script.doesExist("result") << std::endl;
}