/*
 * File: /tomato/tomato/src/LuauGlobals.cpp
 * 
 * Created the 25 June 2024, 11:25 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauGlobals.h"
#include "LuauState.h"

LuauGlobals::LuauGlobals(LuauState &state): m_state(state)
{}

LuauGlobals::LuauGlobals(LuauGlobals&& other): m_state(other.m_state)
{}

LuauGlobals& LuauGlobals::operator=(LuauGlobals&& other) {
    if (this == &other)
        return *this;

    m_state = std::move(other.m_state);
    return *this;
}

lua_Type LuauGlobals::getType(const std::string &globalName) noexcept {
    lua_getglobal(m_state.p_L.get(), globalName.c_str());
    lua_Type type = getTypeInStack(-1);

    lua_pop(m_state.p_L.get(), 1);
    return type;
}

lua_Type LuauGlobals::getTypeInStack(StackIndex idx) noexcept {
    return static_cast<lua_Type>(lua_type(m_state.p_L.get(), idx));
}


bool LuauGlobals::doesExist(const std::string &globalName) noexcept {
    lua_getglobal(m_state.p_L.get(), globalName.c_str());

    bool doesExist = (lua_type(m_state.p_L.get(), -1) != LUA_TNIL);

    lua_pop(m_state.p_L.get(), 1);
    return doesExist;
}