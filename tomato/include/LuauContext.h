#ifndef LUAU_SCRIPT_H_
#define LUAU_SCRIPT_H_

#include <filesystem>
#include <Luau/lua.h>
#include <Luau/lualib.h>
#include <Luau/luacode.h>
#include <string>
#include <any>
#include <list>

namespace tomato {
    namespace fs = std::filesystem;
    
    class LuauContext {
    public:
        LuauContext();
        ~LuauContext();
        void    load(const fs::path scriptPath);
        bool    doesExist(const std::string &globalVarFunc);
        std::list<std::any> runFunction(const std::string &func, std::list<std::any> params, std::size_t resNbr);
        int     call();
        int     run(const fs::path scriptPath);

    private:
        lua_State   *p_L;
    };
}

#endif  // LUAU_SCRIPT_H_