/*
 * File: /src/LuauScript.cpp
 * 
 * Created the 10 April 2023, 06:17 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
 * Copyright 2023 TinyMinori
 */

#include "LuauScript.h"
#include "LuauError.h"

#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstdlib>

static void    *luau_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void) ud;

    if (nsize == 0 && osize != 0 && ptr != nullptr) {
        free(ptr);
        return NULL;
    }
    if (nsize != 0 && osize == 0)
        return malloc(nsize);

    if (osize >= nsize)
        return ptr;

    return realloc(ptr, nsize);
}

LuauScript::LuauScript(const char *scriptPath) {
    this->p_L = lua_newstate(luau_alloc, nullptr);

    if (this->p_L == nullptr)
        throw std::runtime_error("Couldn't create the Luau state.");

    luaL_openlibs(this->p_L);

    std::ifstream   scriptFile("script.luau");
    std::string     source;
    if (!scriptFile.is_open())
        throw std::runtime_error("Couldn't open file '" + std::string(scriptPath) + "'.");
    
    std::string     line;
    while (scriptFile) {
        line = "";
        std::getline(scriptFile, line);
        source += line + '\n';
    }
    scriptFile.close();

    size_t bytecodeSize = 0;
    char    *bytecode = luau_compile(source.c_str(), source.length(), NULL, &bytecodeSize);
    
    int result = luau_load(this->p_L, scriptPath, bytecode, bytecodeSize, 0);
    free(bytecode);

    if (result != 0)
        throw std::runtime_error("Error happened");
}

LuauScript::~LuauScript() {
    lua_close(this->p_L);
}

int     LuauScript::run() {
    lua_call(this->p_L, 0, 0);

    return 0;
}