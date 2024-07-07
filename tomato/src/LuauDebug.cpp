/*
 * File: /tomato/tomato/src/LuauDebug.cpp
 * 
 * Created the 05 July 2024, 10:19 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauDebug.h"
#include <iostream>
#include <iomanip>

LuauDebug::LuauDebug(LuauState& currentState): m_state(currentState)
{}

void LuauDebug::dumpStack() noexcept {
    std::clog << std::boolalpha;
    std::clog << "*************************" << std::endl;
    std::clog << "* Start of stack dump   *" << std::endl;

    int top = lua_gettop(m_state.p_L.get());

    if (top == 0)
        std::clog << "* Empty stack           *" << std::endl;
    
    for (int i = 1; i <= top; i++) {
        std::clog << i << "\t" << std::setfill(' ') << std::setw(10) << luaL_typename(m_state.p_L.get(), i) << "\t";

        switch (lua_type(m_state.p_L.get(), i)) {
            case LUA_TNUMBER:
                std::clog << lua_tonumber(m_state.p_L.get(), i) << std::endl;
                break;
            case LUA_TSTRING:
                std::clog << lua_tostring(m_state.p_L.get(), i) << std::endl;
                break;
            case LUA_TBOOLEAN:
                std::clog << lua_toboolean(m_state.p_L.get(), i) << std::endl;
                break;
            case LUA_TNIL:
                std::clog << "nil" << std::endl;
                break;
            default:
                std::clog << lua_topointer(m_state.p_L.get(), i) << std::endl;
                break;
        }
    }
    std::clog << "* End of stack dump     *" << std::endl;
    std::clog << "*************************" << std::endl;
}