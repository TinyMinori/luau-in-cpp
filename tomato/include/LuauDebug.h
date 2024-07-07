/*
 * File: /tomato/tomato/include/LuauDebug.h
 * 
 * Created the 05 July 2024, 10:18 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include "LuauState.h"

class LuauDebug {
public:
    LuauDebug(LuauState& currentState);
    void dumpStack() noexcept;

private:
    LuauState& m_state;
};