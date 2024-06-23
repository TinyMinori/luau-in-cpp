/*
 * File: /tomato/tomato/src/LuauExceptions.cpp
 * 
 * Created the 25 June 2024, 12:50 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "LuauExceptions.h"

Exception::Exception(LuauExceptions id, const std::string& message)
: m_id(id), m_message(message)
{}

const char* Exception::what() const noexcept {
    return m_message.c_str();
}

LuauExceptions Exception::getId() const noexcept {
    return m_id;
}