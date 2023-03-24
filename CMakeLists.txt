cmake_minimum_required(VERSION 3.13)

project(luau_in_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

link_directories("/usr/lib/Luau")

add_executable(luau_in_cpp main.cpp)

target_link_libraries(luau_in_cpp
    "-l:libLuau.VM.a"
    "-l:libLuau.Compiler.a"
    "-l:libLuau.Ast.a"
)

file(COPY script.luau DESTINATION ${CMAKE_BINARY_DIR})