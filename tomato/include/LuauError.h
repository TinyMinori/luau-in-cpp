/*
 * File: /include/LuauError.h
 * 
 * Created the 11 April 2023, 12:44 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
 * Copyright 2023 TinyMinori
 */

#ifndef LUAU_ERROR_H_
#define LUAU_ERROR_H_

#include "Luau.h"
#include <stdexcept>
#include <memory>

class LuauError: public std::exception {
  public:
    LuauError(lua_State *L);
    LuauError(const LuauError &other);
    LuauError &operator=(const LuauError & other);
    ~LuauError();
    virtual const char *what() const noexcept;
  
  private:
    lua_State   *p_L;
    std::shared_ptr<lua_State>    p_luau_resource;
};

#endif  //LUAU_ERROR_H_