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
    
    struct UserData {
        virtual ~UserData() = default;
    };

    struct LightUserData {
        virtual ~LightUserData() = default;
    };

    class LuauContext {
    public:
        LuauContext();
        ~LuauContext();

        void load(const fs::path scriptPath);
        bool doesExist(const std::string &globalVarFunc);
        std::list<std::any> runFunction(const std::string &func, std::list<std::any> params);
        int call();
        int run(const fs::path scriptPath);

        void push();
        template<typename T> void push(T val);
        template<typename T> T get(StackIndex idx);
        template<typename T> T pop(StackIndex idx);

    private:
        LuauContext(lua_State *threadState);

        lua_State   *p_L;
        void dumpstack();

    };

    template<>
    int LuauContext::get<int>(StackIndex idx) {
        return lua_tointeger(p_L, idx);
    };

    template<>
    double LuauContext::get<double>(StackIndex idx) {
        return lua_tonumber(p_L, idx);
    };

    template<>
    bool LuauContext::get<bool>(StackIndex idx) {
        return lua_toboolean(p_L, idx) > 0;
    };

    template<>
    char *LuauContext::get<char *>(StackIndex idx) {
        return const_cast<char*>(lua_tostring(p_L, idx));
    };

    template<>
    void *LuauContext::get<void *>(StackIndex idx) {
        return const_cast<void *>(lua_topointer(p_L, idx));
    };

    template<>
    UserData *LuauContext::get<UserData *>(StackIndex idx) {
        return static_cast<UserData *>(lua_touserdata(p_L, idx));
    };

    template<>
    LightUserData *LuauContext::get<LightUserData *>(StackIndex idx) {
        return static_cast<LightUserData *>(lua_tolightuserdata(p_L, idx));
    };

    template<>
    lua_State *LuauContext::get<lua_State *>(StackIndex idx) {
        return lua_tothread(p_L, idx);
    };

    template<>
    lua_CFunction LuauContext::get<lua_CFunction>(StackIndex idx) {
        return lua_tocfunction(p_L, idx);
    };

    template<typename T>
    T LuauContext::get(StackIndex idx) {
        static_assert(sizeof(T) == 0, "only specialized methods are usable");
    };

    template<typename T>
    T LuauContext::pop(StackIndex idx) {
        T result = get<T>(idx);
        lua_pop(p_L, 1);

        return result;
    };

    void  LuauContext::push() {
        lua_pushnil(p_L);
    }

    template<>
    void LuauContext::push<bool>(bool val) {
        lua_pushboolean(p_L, val);
    };

    template<>
    void LuauContext::push<int>(int val) {
        lua_pushinteger(p_L, val);
    };

    template<>
    void LuauContext::push<double>(double val) {
        lua_pushnumber(p_L, val);
    };

    template<>
    void LuauContext::push<const char *>(const char * val) {
        lua_pushstring(p_L, val);
    }

    template<>
    void LuauContext::push<LightUserData *>(LightUserData * val) {
        lua_pushlightuserdata(p_L, val);
    }
}

#endif  // LUAU_SCRIPT_H_