project (TomatoTests LANGUAGES CXX)

find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)
include(Catch)

add_library(TomatoTests_sources
    OBJECT
    ./src/test.cpp
)

target_link_libraries(TomatoTests_sources
    PRIVATE tomato Catch2::Catch2
)

target_include_directories(TomatoTests_sources PUBLIC ${TomatoTests_SOURCE_DIR}/include)

add_executable(TomatoTests)

target_sources(TomatoTests PRIVATE
    ./src/main.cpp
)

target_link_libraries(TomatoTests
    PUBLIC TomatoTests_sources
    PRIVATE tomato Catch2::Catch2
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/tests)

catch_discover_tests(TomatoTests)