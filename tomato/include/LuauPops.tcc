/*
 * File: /tomato/tomato/include/LuauPops.tcc
 * 
 * Created the 12 May 2023, 12:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2023 TinyMinori
 */

#ifndef LUAU_POPS_TCC
#define LUAU_POPS_TCC

namespace tomato {
    template<typename T>
    T   LuauContext::pop(StackIndex idx) {
        T result = get<T>(idx);
        lua_pop(p_L, 1);

        return result;
    }
}

#endif