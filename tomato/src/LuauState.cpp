/*
 * File: /tomato/tomato/src/LuauState.cpp
 * 
 * Created the 05 July 2024, 12:09 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauState.h"
#include "LuauGlobals.h"
#include "LuauException.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <map>

void deleteStack(lua_State *state) {
    lua_close(state);
}

template<>
struct std::hash<std::any>
{
    std::size_t operator()(const std::any& s) const noexcept
    {
        std::size_t hash;
        if (s.type().name() == "int")
            hash = std::hash<int>{}(std::any_cast<int>(s));
        if (s.type().name() == "string")
            hash = std::hash<std::string>{}(std::any_cast<std::string>(s));
        if (s.type().name() == "bool")
            hash = std::hash<bool>{}(std::any_cast<bool>(s));
        return hash;
    }
};


LuauState::LuauState(): p_L(luaL_newstate(), deleteStack), m_globals(LuauGlobals(*this)) {
    if (p_L.get() == nullptr)
        throw std::bad_alloc();

    luaL_openlibs(p_L.get());
    luaL_sandbox(p_L.get());
    luaL_sandboxthread(p_L.get());
}

LuauState::LuauState(lua_State *threadState): p_L(threadState, deleteStack), m_globals(LuauGlobals(*this)) {
    if (p_L.get() == nullptr)
        throw std::bad_alloc();
}

LuauState::LuauState(LuauState&& other) : p_L(std::move(other.p_L)), m_globals(LuauGlobals(*this)) { }

LuauState& LuauState::operator=(LuauState&& other) {
    if (this == &other)
        return *this;
    
    if (other.p_L.get() != nullptr)
        p_L = std::move(other.p_L);
    m_globals = LuauGlobals(*this);
    return *this;
}

int     LuauState::call(uint parametersNumber, bool hasMultiReturn) {
    int result = lua_pcall(p_L.get(), parametersNumber, hasMultiReturn ? LUA_MULTRET : 0, 0);

    if (result == LUA_OK)
        return EXIT_SUCCESS;

    std::string errorMsg = "Unknown error message";
    LuauErrorCode errorCode = CallError;

    if (lua_type(p_L.get(), -1) == LUA_TSTRING) {
        size_t  strSize = 0;
        const char *msg = lua_tolstring(p_L.get(), -1, &strSize);
        errorMsg = std::string(msg, strSize);
    }

    if (result == LUA_ERRRUN)
        errorCode = RuntimeError;
    else if (result == LUA_ERRMEM)
        errorCode = MemoryError;
    else if (result == LUA_ERRERR)
        errorCode = ErrorHandlerError;

    throw LuauException(errorCode, errorMsg);
}

std::list<std::any> LuauState::runFunction(const std::string &func, std::list<std::any> params) {
    luaL_sandboxthread(p_L.get());
    int stackSize = lua_gettop(p_L.get());
    lua_getglobal(p_L.get(), func.c_str());
    
    if (!lua_isfunction(p_L.get(), -1)) {
        std::cerr << "function '" << func.c_str() << "' does not exist." << std::endl;
        return std::list<std::any>();
    }

    std::size_t  paramsNumber = 0;
    for (auto arg: params) {
        auto argTypeName = std::string(arg.type().name());

        if (argTypeName == typeid(int).name())
            push(std::any_cast<int>(arg));
        else if (argTypeName == typeid(double).name())
            push(std::any_cast<double>(arg));
        else if (argTypeName == typeid(bool).name())
            push(std::any_cast<bool>(arg) ? 1 : 0);
        else if (argTypeName == typeid(std::string).name())
            push(std::any_cast<const char *>(arg));
        else if (argTypeName == typeid(char *).name())
            push(std::any_cast<const char *>(arg));
        else
            push();
        
        paramsNumber++;
    }

    call(paramsNumber, true);

    std::list<std::any> resultList = {};

    int resNbr = lua_gettop(p_L.get()) - stackSize;
    for (std::size_t i = 0; i < resNbr; i++) {
        std::any result;

        auto resultType = lua_type(p_L.get(), -1);
        if (resultType == LUA_TNONE)
            continue;
        
        switch (resultType) {
            case LUA_TNIL:
                result = NULL;
                break;
            case LUA_TBOOLEAN:
                result = get<bool>(-1);
                break;
            case LUA_TLIGHTUSERDATA:
                result = get<LightUserData *>(-1);
                break;
            case LUA_TUSERDATA:
                result = get<UserData *>(-1);
                break;
            case LUA_TNUMBER:
                result = get<double>(-1);
                break;
            case LUA_TSTRING:
                result = get<char *>(-1);
                break;
            case LUA_TFUNCTION:
                result = get<lua_CFunction>(-1);
                break;
            case LUA_TTHREAD:
                result = get<lua_State*>(-1);
                break;
            default:
                std::cerr << "Missed a result" << std::endl;
        }

        resultList.push_back(result);
        lua_pop(p_L.get(), 1);
    }

    return resultList;
}

LuauGlobals& LuauState::getGlobals() noexcept {
    return m_globals;
}

std::any    LuauState::getVariable(const std::string &varName) {
    lua_getglobal(p_L.get(), varName.c_str());

    std::any variable = getVarInStack(-1);

    lua_pop(p_L.get(), 1);
    return variable;
}

std::any    LuauState::getVarInStack(StackIndex idx) {
    int type = lua_type(p_L.get(), idx);
    if (type == LUA_TNONE)
        return nullptr;

    std::any result;
    switch (type) {
        case LUA_TNIL:
            result = nullptr;
            break;
        case LUA_TBOOLEAN:
            result = get<bool>(idx);
            break;
        case LUA_TLIGHTUSERDATA:
            result = get<LightUserData *>(idx);
            break;
        case LUA_TUSERDATA:
            result = get<UserData *>(idx);
            break;
        case LUA_TNUMBER: {
            double first = get<double>(idx);
            int second = get<int>(idx);

            if (first == second)
                result = second;
            else
                result = first;
            break;
        }
        case LUA_TSTRING:
            result = get<char *>(idx);
            break;
        case LUA_TFUNCTION:
            result = get<lua_CFunction>(idx);
            break;
        case LUA_TTHREAD:
            result = get<lua_State*>(idx);
            break;
        case LUA_TTABLE: {
            int stackSizeStart = lua_gettop(p_L.get());
            int stackPosKey = stackSizeStart + 1;
            int stackPosValue = stackSizeStart + 2;
            push();
            std::map<KeyType, std::any> map;
            while (lua_next(p_L.get(), stackSizeStart) != 0) {
                std::any key = getVarInStack(stackPosKey);
                std::any value = getVarInStack(stackPosValue);

                int keyType = lua_type(p_L.get(), stackPosKey);                    
                if (keyType == LUA_TNUMBER)
                    map.insert(std::pair{ std::any_cast<int>(key), value });
                else if (keyType == LUA_TSTRING)
                    map.insert(std::pair { std::any_cast<char *>(key), value });
                else if (keyType == LUA_TBOOLEAN)
                    map.insert(std::pair { std::any_cast<bool>(key), value });
                lua_pop(p_L.get(), 1);
            }
            result = map;
            break;
        }
        default:
            std::cerr << "Missed a result" << std::endl;
    }
    return result;
}

void  LuauState::push() {
    lua_pushnil(p_L.get());
}