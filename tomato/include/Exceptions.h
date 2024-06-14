/*
 * File: /tomato/tomato/include/Exceptions.h
 * 
 * Created the 14 June 2024, 02:41 pm by TinyMinori
 * Description :
 * 
 * Project repository: https://github.com/TinyMinori/tomato
 * Copyright 2024 TinyMinori
 */

#include <exception>
#include <string>

namespace tomato {
class LuauException: public std::exception {
public:
    LuauException(int id, const std::string& message);
    const char* what() const noexcept;
    int getId() const noexcept;

private:
    const std::string &m_message;
    int m_id;
};

}