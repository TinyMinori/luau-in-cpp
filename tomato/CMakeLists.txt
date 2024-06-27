project(tomato LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb")

set(LUAU_LIBRARY_DIR "/usr/lib/Luau")

add_library(tomato SHARED)

target_sources(tomato PRIVATE
    ./src/LuauExceptions.cpp
    ./src/LuauReader.cpp
    ./src/LuauGlobals.cpp
    ./src/LuauState.cpp
)

find_library(Luau.VM NAMES Luau.VM PATHS ${LUAU_LIBRARY_DIR})
find_library(Luau.Compiler NAMES Luau.Compiler PATHS ${LUAU_LIBRARY_DIR})
find_library(Luau.Ast NAMES Luau.Ast PATHS ${LUAU_LIBRARY_DIR})

target_link_libraries(
    tomato
    LINK_INTERFACE_LIBRARIES
    ${Luau.VM}
    ${Luau.Compiler}
    ${Luau.Ast}
)

target_include_directories(tomato PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)