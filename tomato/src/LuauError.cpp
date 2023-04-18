/*
 * File: /src/LuauError.cpp
 * 
 * Created the 11 April 2023, 12:49 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
 * Copyright 2023 TinyMinori
 */

#include "LuauError.h"

static void luau_resource_delete(lua_State *L) {
    lua_pop(L, 1);
}

LuauError::LuauError(lua_State *L)
    : std::exception(), p_L(L), p_luau_resource(L, luau_resource_delete)
{}

LuauError::LuauError(const LuauError &other)
    : std::exception(), p_L(other.p_L), p_luau_resource(other.p_luau_resource)
{}

LuauError &LuauError::operator=(const LuauError & other) {
    p_L = other.p_L;
    p_luau_resource = other.p_luau_resource;
    
    return *this;
}

LuauError::~LuauError() {}

const char *LuauError::what() const noexcept {
    const char *s = lua_tostring(this->p_L, -1);

    if (s == nullptr)
        s = "unrecognized Lua error";
    return s;
}