project(demo)

set(CMAKE_CXX_FLAGS "-g3 -ggdb")

add_executable(demo)

target_sources(demo PRIVATE
    main.cpp
)

target_link_libraries(demo PRIVATE tomato)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/script.luau script.luau COPYONLY)