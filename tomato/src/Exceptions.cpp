/*
 * File: /tomato/tomato/src/Exceptions.cpp
 * 
 * Created the 14 June 2024, 02:41 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include "Exceptions.h"

namespace tomato {

LuauException::LuauException(LuauExceptions id, const std::string& message)
: m_id(id), m_message(message)
{}

const char* LuauException::what() const noexcept {
    return m_message.c_str();
}

LuauExceptions LuauException::getId() const noexcept {
    return m_id;
}

}