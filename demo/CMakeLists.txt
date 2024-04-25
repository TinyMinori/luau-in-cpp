project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb")

add_executable(demo)

target_sources(demo PRIVATE
    src/main.cpp
)

target_link_libraries(demo PRIVATE tomato)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/demo)