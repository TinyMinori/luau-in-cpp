#include "LuauContext.h"

#include <iostream>
#include <stdexcept>
#include <fstream>

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

    LuauContext::LuauContext(lua_State *threadState) {
        p_L = threadState;

        if (p_L == nullptr)
            throw std::bad_alloc();
    }

    LuauContext::~LuauContext() {
        lua_close(p_L);
    }

    void    LuauContext::load(const fs::path scriptPath) {
        // File checks
        if (!fs::is_regular_file(scriptPath))
            throw std::runtime_error(std::string("The script named") + scriptPath.c_str() + "is not a correct script location.");

        std::ifstream   scriptFile(scriptPath);
        std::string     source;
        if (!scriptFile.is_open())
            throw std::runtime_error(std::string("Couldn't open file") + scriptPath.c_str() + ".");
        
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
            dumpstack();
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
        //dumpstack();

        int callResult = lua_pcall(p_L, paramsNumber, resNbr, 0);
        
        if (callResult != LUA_OK) {
            dumpstack();

            throw std::runtime_error(lua_tostring(p_L, -1));
        }

        std::list<std::any> resultList = {};

        //dumpstack();
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
    
    void    LuauContext::dumpstack() {
        std::clog << std::boolalpha;
        std::clog << "-- Start of stack dump --" << std::endl;
        int top = lua_gettop(p_L);

        if (top == 0)
            std::clog << "-- Empty stack --" << std::endl;

        for (int i = 0; i < top; i++) {
            std::clog << i << "\t" << std::setfill(' ') << std::setw(10) << luaL_typename(p_L, i) << "\t";

            switch (lua_type(p_L, i)) {
                case LUA_TNUMBER:
                    std::clog << lua_tonumber(p_L, i) << std::endl;
                    break;
                case LUA_TSTRING:
                    std::clog << lua_tostring(p_L, i) << std::endl;
                    break;
                case LUA_TBOOLEAN:
                    std::clog << (lua_toboolean(p_L, i) > 0) << std::endl;
                    break;
                case LUA_TNIL:
                    std::clog << "nil" << std::endl;
                    break;
                default:
                    std::clog << lua_topointer(p_L, i) << std::endl;
                    break;
            }
        }
        std::clog << "-- End of stack dump --" << std::endl;
    }
}