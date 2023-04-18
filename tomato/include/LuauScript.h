/*
 * File: /include/LuauScript.h
 * 
 * Created the 10 April 2023, 06:17 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori
 * Copyright 2023 TinyMinori
 */

#ifndef LUAU_SCRIPT_H_
#define LUAU_SCRIPT_H_

#include "Luau.h"

class LuauScript {
  public:
    LuauScript(const char *scriptPath);
    ~LuauScript();
    int     run();

  private:
    lua_State   *p_L;
};

#endif  // LUAU_SCRIPT_H_