/*
 * File: /tomato/tomato/include/LuauContext.h
 * 
 * Created the 04 May 2023, 11:31 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#ifndef LUAU_SCRIPT_H_
#define LUAU_SCRIPT_H_

#include <filesystem>
#include "Luau/lua.h"
#include "Luau/lualib.h"
#include "Luau/luacode.h"
#include <string>
#include <any>
#include <list>
#include <concepts>

namespace tomato {
    namespace fs = std::filesystem;
    
    typedef int64_t StackIndex;
    typedef void*   UserData;

    class LuauContext {
    public:
        LuauContext();
        ~LuauContext();

        void load(const fs::path scriptPath);
        bool doesExist(const std::string &globalVarFunc);
        std::list<std::any> runFunction(const std::string &func, std::list<std::any> params);
        int call();
        int run(const fs::path scriptPath);
        void dumpstack();

    private:
        LuauContext(lua_State *threadState);

        lua_State   *p_L;

        template<typename T>
        T get(StackIndex idx) {
            static_assert(sizeof(T) == 0, "only specialized methods are usable");
        };

        template <int>
        int get(StackIndex idx) {
            return lua_tointeger(p_L, idx);
        };

        template <double>
        double get(StackIndex idx) {
            return lua_tonumber(p_L, idx);
        };

        template <bool>
        bool get(StackIndex idx) {
            return lua_toboolean(p_L, idx) > 0;
        };

        template <const char*>
        const char *get(StackIndex idx) {
            return lua_tostring(p_L, idx);
        };

        template <void*>
        void *get(StackIndex idx) {
            return lua_topointer(p_L, idx);
        };

        template<typename T>
        T pop(StackIndex idx) {
            T result = get<T>(idx);
            lua_pop(p_L, 1);

            return result;
        };

        void push() {
            lua_pushnil(p_L);
        };

        void push(bool val) {
            lua_pushboolean(p_L, val);
        };

        void push(int val) {
            lua_pushinteger(p_L, val);
        };

        void push(double val) {
            lua_pushnumber(p_L, val);
        };

        void push(const char *val) {
            lua_pushstring(p_L, val);
        };
    };
}

#endif  // LUAU_SCRIPT_H_