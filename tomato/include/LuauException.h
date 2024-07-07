/*
 * File: /tomato/tomato/include/LuauException.h
 * 
 * Created the 14 June 2024, 02:41 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#pragma once

#include <exception>
#include <string>

enum LuauErrorCode {
    FileDoesNotExist,
    FileNotRegular,
    FileRightsError,

    CompileSyntaxError,
    LoadError,
    CallError,

    RuntimeError,
    MemoryError,
    ErrorHandlerError,

    StateNotInitialized,

    IncorrectType,
};

class LuauException: public std::exception {
public:
    LuauException(LuauErrorCode id, const std::string& message);
    const char* what() const noexcept;
    LuauErrorCode getId() const noexcept;

private:
    const std::string &m_message;
    LuauErrorCode m_id;
};