/*
 * File: /luau-in-cpp/main.cpp
 * 
 * Created the 22 March 2023, 10:01 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
 * Copyright 2023 TinyMinori
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <Luau/lua.h>
#include <Luau/luacode.h>
#include <Luau/lualib.h>

void    *luau_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
    (void) ud;

    if (nsize == 0 && osize != 0 && ptr != nullptr) {
        free(ptr);
        return NULL;
    }
    if (nsize != 0 && osize == 0)
        return malloc(nsize);

    if (osize >= nsize);
        return ptr;

    return realloc(ptr, nsize);
}

int main() {
    lua_State *L = lua_newstate(luau_alloc, nullptr);
    luaL_openlibs(L);

    if (L == nullptr) {
        std::cout << "couldn't create the luau state";
        return 0;
    }

    std::ifstream   scriptFile("script.luau");
    std::string     source;
    if (!scriptFile.is_open()) {
        std::cout << "Couldn't open file\n";
        return -128;
    }
    
    std::string     line;
    while (scriptFile) {
        line = "";
        std::getline(scriptFile, line);
        source += line + '\n';
    }
    scriptFile.close();

    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(source.c_str(), source.length(), NULL, &bytecodeSize);
    
    int result = luau_load(L, "Script Test", bytecode, bytecodeSize, 0);
    free(bytecode);

    lua_call(L, 0, 0);

    lua_close(L);

    return result;
}