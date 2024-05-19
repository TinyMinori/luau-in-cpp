project (TomatoTests LANGUAGES CXX)

find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)
include(Catch)

add_executable(TomatoTests)

target_sources(TomatoTests PRIVATE
    ./src/test.cpp
    ./src/moveOperatorTests.cpp
)

target_link_libraries(TomatoTests
    PRIVATE tomato Catch2::Catch2WithMain
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/tests)

catch_discover_tests(TomatoTests)