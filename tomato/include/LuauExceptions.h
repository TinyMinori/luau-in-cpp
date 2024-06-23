/*
 * File: /tomato/tomato/include/LuauExceptions.h
 * 
 * Created the 25 June 2024, 12:50 am by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <exception>
#include <string>

enum LuauExceptions {
    FileDoesNotExist,
    FileNotRegular,
    FileRightsError,

    CompileSyntaxError,
    LoadError,

    StateNotInitialized
};

class Exception: public std::exception {
public:
    Exception(LuauExceptions id, const std::string& message);
    const char* what() const noexcept;
    LuauExceptions getId() const noexcept;

private:
    const std::string &m_message;
    LuauExceptions m_id;
};