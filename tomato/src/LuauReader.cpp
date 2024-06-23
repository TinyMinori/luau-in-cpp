/*
 * File: /tomato/tomato/src/LuauReader.cpp
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <fstream>
#include <cstring>
#include "LuauReader.h"
#include "LuauExceptions.h"
#include "Luau/lua.h"
#include "Luau/luacode.h"

LuauState LuauReader::getContextFromFile(const fs::path &filepath) noexcept(false) {
    if (!fs::exists(filepath))
        throw Exception(LuauExceptions::FileDoesNotExist, std::string("The file ") + filepath.c_str() + " doesn't exist.");
    
    if (!fs::is_regular_file(filepath))
        throw Exception(LuauExceptions::FileNotRegular, std::string("The file ") + filepath.c_str() + " is not a regular file.");

    std::ifstream   scriptFile(filepath);
    std::string     source;
    if (!scriptFile.is_open())
        throw Exception(LuauExceptions::FileRightsError, std::string("Couldn't open file ") + filepath.c_str() + ".");
    
    std::string     line;
    while (scriptFile) {
        line = "";
        std::getline(scriptFile, line);
        source += line + '\n';
    }
    scriptFile.close();

    return getContextFromText(source, filepath.c_str());
}

LuauState LuauReader::getContextFromText(const std::string &luauScript, const std::string &chuckName) noexcept(false) {
    auto luauContext = LuauState();

    if (luauContext.p_L.get() == nullptr)
        throw Exception(LuauExceptions::StateNotInitialized, "state not inizialised");

    // Compile and load
    size_t  bytecodeSize = 0;
    char    *bytecode = luau_compile(luauScript.c_str(), luauScript.length(), NULL, &bytecodeSize);

    if (bytecode == nullptr || strlen(bytecode) == 0)
        throw Exception(LuauExceptions::CompileSyntaxError, "Syntax Error");
    
    int result = luau_load(luauContext.p_L.get(), chuckName.c_str(), bytecode, bytecodeSize, 0);
    free(bytecode);

    if (result != LUA_OK)
        throw Exception(LuauExceptions::LoadError, "error #" + std::to_string(result));
    
    luauContext.call();

    return luauContext;
}