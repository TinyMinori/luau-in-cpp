/*
 * File: /tomato/tomato/include/LuauContext.h
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#ifndef LUAU_SCRIPT_H_
#define LUAU_SCRIPT_H_

#include <filesystem>
#include <string>
#include <any>
#include <list>
#include <concepts>
#include <memory>
#include <variant>
#include <map>
#include "Luau/lua.h"
#include "Luau/lualib.h"
#include "Luau/luacode.h"

namespace tomato {
    #define STATE_NOT_INIALIZED 255

    namespace fs = std::filesystem;

    struct UserData {};
    
    struct LightUserData {};

    typedef int64_t StackIndex;

    typedef std::variant<bool, int, char*> KeyType;

    class LuauContext {
    public:
        LuauContext();

        void load(const fs::path scriptPath);
        bool doesExist(const std::string &globalVarFunc) noexcept;
        std::list<std::any> runFunction(const std::string &func, std::list<std::any> params);
        std::any getVariable(const std::string &varName);
        std::any getVarInStack(StackIndex idx);
        int call();
        int run(const fs::path scriptPath);
        
        void push();
        template<typename T> void push(T val);
        template<typename T> T get(StackIndex idx);
        template<typename T> T pop(StackIndex idx);

        LuauContext(const LuauContext& other) = delete;
        LuauContext(LuauContext&& other);
        LuauContext& operator=(const LuauContext& other) = delete;
        LuauContext& operator=(LuauContext&& other);
    private:
        LuauContext(lua_State *threadState);

        std::unique_ptr<lua_State, void(*)(lua_State*)>  p_L;
        void dumpstack() noexcept;   
    };

    template<typename T>
    inline T LuauContext::get(StackIndex idx) {
        static_assert(sizeof(T) == 0, "only specialized methods are usable");
    };

    template<>
    inline int LuauContext::get<int>(StackIndex idx) {
        return lua_tointeger(p_L.get(), idx);
    };

    template<>
    inline double LuauContext::get<double>(StackIndex idx) {
        return lua_tonumber(p_L.get(), idx);
    };

    template<>
    inline bool LuauContext::get<bool>(StackIndex idx) {
        return lua_toboolean(p_L.get(), idx) > 0;
    };

    template<>
    inline char *LuauContext::get<char *>(StackIndex idx) {
        return const_cast<char*>(lua_tostring(p_L.get(), idx));
    };

    template<>
    inline void *LuauContext::get<void *>(StackIndex idx) {
        return const_cast<void *>(lua_topointer(p_L.get(), idx));
    };

    template<>
    inline UserData *LuauContext::get<UserData *>(StackIndex idx) {
        return static_cast<UserData *>(lua_touserdata(p_L.get(), idx));
    };

    template<>
    inline LightUserData *LuauContext::get<LightUserData *>(StackIndex idx) {
        return static_cast<LightUserData *>(lua_tolightuserdata(p_L.get(), idx));
    };

    template<>
    inline lua_State *LuauContext::get<lua_State *>(StackIndex idx) {
        return lua_tothread(p_L.get(), idx);
    };

    template<>
    inline lua_CFunction LuauContext::get<lua_CFunction>(StackIndex idx) {
        return lua_tocfunction(p_L.get(), idx);
    };

    template<typename T>
    inline T LuauContext::pop(StackIndex idx) {
        T result = get<T>(idx);
        lua_pop(p_L.get(), 1);

        return result;
    };

    template<>
    inline void LuauContext::push<bool>(bool val) {
        lua_pushboolean(p_L.get(), val);
    };

    template<>
    inline void LuauContext::push<int>(int val) {
        lua_pushinteger(p_L.get(), val);
    };

    template<>
    inline void LuauContext::push<double>(double val) {
        lua_pushnumber(p_L.get(), val);
    };

    template<>
    inline void LuauContext::push<const char *>(const char * val) {
        lua_pushstring(p_L.get(), val);
    };

    template<>
    inline void LuauContext::push<LightUserData *>(LightUserData * val) {
        lua_pushlightuserdata(p_L.get(), val);
    };
}

#endif  // LUAU_SCRIPT_H_