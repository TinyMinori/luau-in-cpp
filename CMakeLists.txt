cmake_minimum_required(VERSION 3.5)
find_package(Catch2 3 REQUIRED)

project(tomato-demo)

set(CMAKE_VERBOSE_MAKEFILE OFF)

add_subdirectory(tomato)
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(demo)
    add_subdirectory(tests)
endif()

enable_testing()