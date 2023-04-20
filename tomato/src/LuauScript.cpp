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

LuauScript::LuauScript(const fs::path scriptPath) {
    // File checks
    if (!fs::is_regular_file(scriptPath))
        throw std::runtime_error("The script named '" + scriptPath.u8string() +  "' is not a correct script location");
    
    auto scriptLocation = scriptPath;
    if (scriptLocation.is_relative())
        scriptLocation = fs::absolute(scriptLocation);

    std::ifstream   scriptFile(scriptLocation);
    std::string     source;
    if (!scriptFile.is_open())
        throw std::runtime_error("Couldn't open file '" + scriptLocation.u8string() + "'.");
    
    std::string     line;
    while (scriptFile) {
        line = "";
        std::getline(scriptFile, line);
        source += line + '\n';
    }
    scriptFile.close();

    // Luau creating
    this->p_L = lua_newstate(luau_alloc, nullptr);

    if (this->p_L == nullptr)
        throw std::runtime_error("Couldn't create the Luau state.");

    luaL_openlibs(this->p_L);

    size_t bytecodeSize = 0;
    char    *bytecode = luau_compile(source.c_str(), source.length(), NULL, &bytecodeSize);
    
    int result = luau_load(this->p_L, scriptLocation.c_str(), bytecode, bytecodeSize, 0);
    free(bytecode);

    if (result == LUA_ERRSYNTAX)
        throw std::runtime_error("Syntax error");
    if (result == LUA_ERRMEM)
        throw std::runtime_error("Memory error");
}

LuauScript::~LuauScript() {
    lua_close(this->p_L);
}

int     LuauScript::run() {
    int result = lua_pcall(this->p_L, 0, 0, 0);

    if (result == LUA_ERRRUN)
        std::cerr << "Runtime error" << std::endl;
    if (result == LUA_ERRMEM)
        std::cerr << "Memory error" << std::endl;
    if (result == LUA_ERRERR)
        std::cerr << "Unknown error" << std::endl;
    return result;
}