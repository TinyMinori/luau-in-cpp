/*
 * File: /tomato/tomato/include/LuauGlobals.h
 * 
 * Created the 26 June 2024, 12:04 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <string>
#include <memory>

#include <Luau/lua.h>

class LuauState;

typedef int64_t StackIndex;

class LuauGlobals {
public:
    friend class LuauState;
    
    LuauGlobals(const LuauGlobals& other) = delete;
    LuauGlobals(LuauGlobals&& other);

    bool doesExist(const std::string &globalName) noexcept;
    lua_Type getType(const std::string &globalName) noexcept;

    LuauGlobals& operator=(const LuauGlobals& other) = delete;
    LuauGlobals& operator=(LuauGlobals&& other);

protected:
    LuauGlobals(LuauState &state);

    lua_Type getTypeInStack(StackIndex idx) noexcept;

private:
    LuauState &m_state;
};