#include "LuauContext.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

void dumpstack(lua_State *L) {
    printf("Stack Dump\n");
    int top = lua_gettop(L);

    for (int i = 1; i <= top; i++) {
        printf("%d\t%-10s\t", i, luaL_typename(L, i));

        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                printf("%g\n", lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                printf("%s\n", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n", lua_topointer(L, i));
                break;
        }
    }
}

namespace tomato {

    LuauContext::LuauContext() {
        // Luau state creation
        p_L = luaL_newstate();

        if (p_L == nullptr)
            throw std::bad_alloc();

        luaL_openlibs(p_L);
        luaL_sandbox(p_L);
        luaL_sandboxthread(p_L);
    }

    LuauContext::~LuauContext() {
        lua_close(p_L);
    }

    void    LuauContext::load(const fs::path scriptPath) {
        // File checks
        if (!fs::is_regular_file(scriptPath))
            throw std::runtime_error("The script named '" + scriptPath.u8string() +  "' is not a correct script location");

        std::ifstream   scriptFile(scriptPath);
        std::string     source;
        if (!scriptFile.is_open())
            throw std::runtime_error("Couldn't open file '" + scriptPath.u8string() + "'.");
        
        std::string     line;
        while (scriptFile) {
            line = "";
            std::getline(scriptFile, line);
            source += line + '\n';
        }
        scriptFile.close();

        // Compile and load
        size_t bytecodeSize = 0;
        char    *bytecode = luau_compile(source.c_str(), source.length(), NULL, &bytecodeSize);

        int result = luau_load(p_L, scriptPath.c_str(), bytecode, bytecodeSize, 0);
        free(bytecode);

        if (result == LUA_ERRSYNTAX)
            throw std::runtime_error("Syntax error");
        if (result == LUA_ERRMEM)
            throw std::runtime_error("Memory error");   
    }

    int     LuauContext::call() {
        // Call
        int result = lua_pcall(p_L, 0, 0, 0);

        if (result == LUA_OK)
            return EXIT_SUCCESS;

        std::string errorMsg; 
        if (lua_type(p_L, -1) == LUA_TSTRING) {
            size_t  strSize = 0;
            dumpstack(p_L);
            const char *msg = lua_tolstring(p_L, -1, &strSize);
            errorMsg = std::string(msg, strSize);
        }

        if (errorMsg.empty())
            throw std::runtime_error("Unknown error message");

        if (result == LUA_ERRRUN)
            throw std::runtime_error(errorMsg);
        if (result == LUA_ERRMEM)
            throw std::out_of_range(errorMsg);
        if (result == LUA_ERRERR)
            throw std::logic_error(errorMsg);
        return result;
    }

    int     LuauContext::run(const fs::path scriptPath) {
        load(scriptPath);
        return call();
    }
    
    bool    LuauContext::doesExist(const std::string &globalVarFunc) {
        lua_getglobal(p_L, globalVarFunc.c_str());

        bool doesExist = (lua_type(p_L, -1) != LUA_TNIL);

        lua_pop(p_L, 1);
        return doesExist;
    }

    std::list<std::any> LuauContext::runFunction(const std::string &func, std::list<std::any> params, std::size_t resNbr) {
        lua_getglobal(p_L, func.c_str());
        
        if (lua_type(p_L, -1) == LUA_TNIL) {
            std::cerr << "function '" << func.c_str() << "' does not exist." << std::endl;
            return std::list<std::any>();
        }

        std::size_t  paramsNumber = 0;
        for (auto arg: params) {
            auto argTypeName = std::string(arg.type().name());

            if (argTypeName == typeid(int).name())
                lua_pushinteger(p_L, std::any_cast<int>(arg));
            else if (argTypeName == typeid(double).name())
                lua_pushnumber(p_L, std::any_cast<double>(arg));
            else if (argTypeName == typeid(bool).name())
                lua_pushboolean(p_L, std::any_cast<bool>(arg) ? 1 : 0);
            else if (argTypeName == typeid(std::string).name())
                lua_pushstring(p_L, std::any_cast<const char *>(arg));
            else if (argTypeName == typeid(char *).name())
                lua_pushstring(p_L, std::any_cast<const char *>(arg));
            else
                lua_pushnil(p_L);
            
            paramsNumber++;
        }
        //dumpstack(p_L);

        int callResult = lua_pcall(p_L, paramsNumber, resNbr, 0);
        
        if (callResult != LUA_OK) {
            dumpstack(p_L);

            throw std::runtime_error(lua_tostring(p_L, -1));
        }

        std::list<std::any> resultList = {};

        //dumpstack(p_L);
        for (std::size_t i = 0; i < resNbr; i++) {
            std::any result;

            auto resultType = lua_type(p_L, -1);
            if (resultType == LUA_TNONE)
                continue;
            
            switch (lua_type(p_L, -1)) {
                case LUA_TNIL:
                    result = NULL;
                    break;
                case LUA_TBOOLEAN:
                    result = lua_toboolean(p_L, -1) != 0;
                    break;
                case LUA_TLIGHTUSERDATA:
                    result = lua_tolightuserdata(p_L, -1);
                    break;
                case LUA_TNUMBER:
                    result = lua_tonumber(p_L, -1);
                    break;
                case LUA_TSTRING:
                    result = const_cast<char *>(lua_tostring(p_L, -1));
                    break;
                case LUA_TFUNCTION:
                    result = lua_tocfunction(p_L, -1);
                    break;
                case LUA_TUSERDATA:
                    result = lua_touserdata(p_L, -1);
                    break;
                case LUA_TTHREAD:
                    result = lua_tothread(p_L, -1);
                    break;
                default:
                    std::cerr << "Missed a result" << std::endl;
            }

            resultList.push_back(result);                
            lua_pop(p_L, 1);
        }

        return resultList;
    }
    
}