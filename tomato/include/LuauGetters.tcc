/*
 * File: /tomato/tomato/include/LuauGetters.tcc
 * 
 * Created the 11 May 2023, 11:48 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2023 TinyMinori
 */

#ifndef LUAU_GETTERS_TCC
#define LUAU_GETTERS_TCC

namespace tomato {
    template<typename T>
    T   LuauContext::get(StackIndex idx) {
        static_assert(sizeof(T) == 0, "only specialized methods are usable");
    }

    template <>
    int LuauContext::get(StackIndex idx) {
        return lua_tointeger(p_L, idx);
    }

    template <>
    bool LuauContext::get(StackIndex idx) {
        return lua_toboolean(p_L, idx) > 0;
    }

    template <>
    const char *LuauContext::get(StackIndex idx) {
        return lua_tostring(p_L, idx);
    }
}

#endif