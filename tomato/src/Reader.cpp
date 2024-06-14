/*
 * File: /tomato/tomato/src/Reader.cpp
 * 
 * Created the 14 June 2024, 12:02 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <fstream>
#include "Reader.h"
#include "Exceptions.h"
#include "Luau/lua.h"
#include "Luau/luacode.h"

namespace tomato {

LuauContext LuauReader::getContextFromFile(const fs::path &filepath) {
    if (!fs::is_regular_file(filepath))
        throw std::runtime_error(std::string("The script named ") + filepath.c_str() + " is not a correct script location.");

    std::ifstream   scriptFile(filepath);
    std::string     source;
    if (!scriptFile.is_open())
        throw std::runtime_error(std::string("Couldn't open file ") + filepath.c_str() + ".");
    
    std::string     line;
    while (scriptFile) {
        line = "";
        std::getline(scriptFile, line);
        source += line + '\n';
    }
    scriptFile.close();

    return getContextFromText(source, filepath.c_str());
}

LuauContext LuauReader::getContextFromText(const std::string &luauScript, const std::string &chuckName) {
    auto luauContext = LuauContext();

    if (luauContext.p_L.get() == nullptr)
        throw LuauException(STATE_NOT_INITIALIZED, "state not inizialised");

    // Compile and load
    size_t  bytecodeSize = 0;
    char    *bytecode = luau_compile(luauScript.c_str(), luauScript.length(), NULL, &bytecodeSize);

    int result = luau_load(luauContext.p_L.get(), chuckName.c_str(), bytecode, bytecodeSize, 0);
    free(bytecode);

    if (result == LUA_ERRSYNTAX)
        throw std::runtime_error("Syntax error");
    if (result == LUA_ERRMEM)
        throw std::runtime_error("Memory error");
    if (result != LUA_OK)
        throw std::runtime_error("error #" + std::to_string(result));
    luauContext.call();
    return luauContext;
}

}