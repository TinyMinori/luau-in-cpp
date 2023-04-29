/*
 * File: /demo/main.cpp
 * 
 * Created the 22 March 2023, 10:01 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
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
        return 1;
    }
    tomato::LuauScript  script { argv[1] };

    return script.run();
}