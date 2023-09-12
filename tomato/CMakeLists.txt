project(tomato LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb")

add_library(tomato SHARED)

target_sources(tomato PRIVATE
    ./include/LuauContext.h

    ./src/LuauContext.cpp
)

find_library(Luau.VM NAMES Luau.VM PATHS "/usr/lib/Luau")
find_library(Luau.Compiler NAMES Luau.Compiler PATHS "/usr/lib/Luau")
find_library(Luau.Ast NAMES Luau.Ast PATHS "/usr/lib/Luau")

target_link_libraries(
    tomato
    INTERFACE
    ${Luau.VM}
    ${Luau.Compiler}
    ${Luau.Ast}
)

target_include_directories(tomato PUBLIC ${tomato_SOURCE_DIR}/include)