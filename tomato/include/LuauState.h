/*
 * File: /tomato/tomato/include/LuauState.h
 * 
 * Created the 20 May 2024, 01:36 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <string>
#include <any>
#include <list>
#include <concepts>
#include <memory>
#include <variant>
#include <map>
#include <Luau/lua.h>
#include <Luau/lualib.h>
#include <Luau/luacode.h>
#include "LuauGlobals.h"

struct UserData {};

struct LightUserData {};

typedef int64_t StackIndex;

typedef std::variant<bool, int, char*> KeyType;

class LuauState {
public:
    friend class LuauReader;
    friend class LuauGlobals;

    LuauState(const LuauState& other) = delete;
    LuauState(LuauState&& other);
    
    LuauState& operator=(const LuauState& other) = delete;
    LuauState& operator=(LuauState&& other);

    std::list<std::any> runFunction(const std::string &func, std::list<std::any> params);

    LuauGlobals& getGlobals() noexcept;

    std::any getVariable(const std::string &varName);
    std::any getVarInStack(StackIndex idx);
    
    void push();
    template<typename T> void push(T val);
    template<typename T> T get(StackIndex idx);
    template<typename T> T pop(StackIndex idx);

private:
    LuauState();
    LuauState(lua_State *threadState);

    int call();
    std::unique_ptr<lua_State, void(*)(lua_State*)>  p_L;
    LuauGlobals m_globals;
    void dumpstack() noexcept;   
};

template<typename T>
inline T LuauState::get(StackIndex idx) {
    static_assert(sizeof(T) == 0, "only specialized methods are usable");
};

template<>
inline int LuauState::get<int>(StackIndex idx) {
    return lua_tointeger(p_L.get(), idx);
};

template<>
inline double LuauState::get<double>(StackIndex idx) {
    return lua_tonumber(p_L.get(), idx);
};

template<>
inline bool LuauState::get<bool>(StackIndex idx) {
    return lua_toboolean(p_L.get(), idx) > 0;
};

template<>
inline char *LuauState::get<char *>(StackIndex idx) {
    return const_cast<char*>(lua_tostring(p_L.get(), idx));
};

template<>
inline void *LuauState::get<void *>(StackIndex idx) {
    return const_cast<void *>(lua_topointer(p_L.get(), idx));
};

template<>
inline UserData *LuauState::get<UserData *>(StackIndex idx) {
    return static_cast<UserData *>(lua_touserdata(p_L.get(), idx));
};

template<>
inline LightUserData *LuauState::get<LightUserData *>(StackIndex idx) {
    return static_cast<LightUserData *>(lua_tolightuserdata(p_L.get(), idx));
};

template<>
inline lua_State *LuauState::get<lua_State *>(StackIndex idx) {
    return lua_tothread(p_L.get(), idx);
};

template<>
inline lua_CFunction LuauState::get<lua_CFunction>(StackIndex idx) {
    return lua_tocfunction(p_L.get(), idx);
};

template<typename T>
inline T LuauState::pop(StackIndex idx) {
    T result = get<T>(idx);
    lua_pop(p_L.get(), 1);

    return result;
};

template<>
inline void LuauState::push<bool>(bool val) {
    lua_pushboolean(p_L.get(), val);
};

template<>
inline void LuauState::push<int>(int val) {
    lua_pushinteger(p_L.get(), val);
};

template<>
inline void LuauState::push<double>(double val) {
    lua_pushnumber(p_L.get(), val);
};

template<>
inline void LuauState::push<const char *>(const char * val) {
    lua_pushstring(p_L.get(), val);
};

template<>
inline void LuauState::push<LightUserData *>(LightUserData * val) {
    lua_pushlightuserdata(p_L.get(), val);
};