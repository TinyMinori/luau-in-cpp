/*
 * File: /tomato/demo/src/main.cpp
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauContext.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

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
    tomato::LuauContext script{};

    std::cout << std::boolalpha;
    script.run(argv[1]);

    auto isPositive = script.getVariable("isPositiveVar");
    std::cout << "[C] Is positive variable: " << std::any_cast<bool>(isPositive) << std::endl << std::endl;

    bool arrTestExists = script.doesExist("arrayTest");
    std::cout << "[C] arrayTest does exist : " << arrTestExists << std::endl;

    auto arrTest = script.getVariable("arrayTest");
    std::map<tomato::KeyType, std::any> map = std::any_cast<std::map<tomato::KeyType, std::any>>(arrTest);
    std::cout << "[C] arrayTest variable: {";
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto i = std::distance(map.begin(), it);

        try {
            std::cout << std::get<int>(it->first);
        } catch (const std::bad_variant_access& ex) {
            try {
                std::cout << std::get<bool>(it->first);
            } catch (const std::bad_variant_access& ex) {
                std::cout << "\"" << std::get<char *>(it->first) << "\"";                
            }
        }
        std::cout << " to ";
        if (it->second.type().hash_code() == typeid(int).hash_code())
            std::cout << std::any_cast<int>(it->second);
        else if (it->second.type().hash_code() == typeid(bool).hash_code())
            std::cout << std::any_cast<bool>(it->second);
        else if (it->second.type().hash_code() == typeid(char*).hash_code())
            std::cout << "\"" << std::any_cast<char *>(it->second) << "\"";
        else
            std::cout << "null";

        if ((i + 1) < map.size())
            std::cout << ", ";
    }
    std::cout << "}" << std::endl << std::endl;

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