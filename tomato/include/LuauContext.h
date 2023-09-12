#ifndef LUAU_SCRIPT_H_
#define LUAU_SCRIPT_H_

#include <filesystem>
#include <Luau/lua.h>
#include <Luau/lualib.h>
#include <Luau/luacode.h>
#include <string>
#include <any>
#include <list>
#include <concepts>

namespace tomato {
    namespace fs = std::filesystem;
    
    typedef int64_t     StackIndex;
    typedef void*       UserData;

    class LuauContext;

    /*template<typename T>
    concept AcceptedTypes = std::is_same<T, bool>::value
                            | std::is_same<T, int>::value
                            | std::is_same<T, float>::value
                            | std::is_same<T, double>::value
                            | std::is_same<T, const char*>::value
                            | std::is_base_of<T, LuauContext>::value
                            | std::is_base_of<T, UserData>::value
                            | std::is_base_of<T, lua_CFunction>::value
                            | std::is_null_pointer<T>::value;*/

    class LuauContext {
    public:
        LuauContext();
        ~LuauContext();
        template<typename T>
        T       get(StackIndex idx);

        template<int>
        int     get(StackIndex idx);

        template<bool>
        bool    get(StackIndex idx);

        template<const char *>
        const char  *get(StackIndex idx);

        template<typename T>
        T       pop(StackIndex idx);

        void    load(const fs::path scriptPath);
        bool    doesExist(const std::string &globalVarFunc);
        std::list<std::any> runFunction(const std::string &func, std::list<std::any> params, std::size_t resNbr);
        int     call();
        int     run(const fs::path scriptPath);
        void    dumpstack();

    private:
        LuauContext(lua_State *threadState);

        lua_State   *p_L;
    };
}

#include "LuauGetters.tcc"
#include "LuauPops.tcc"

#endif  // LUAU_SCRIPT_H_