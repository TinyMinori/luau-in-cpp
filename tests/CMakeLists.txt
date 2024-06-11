project (TomatoTests LANGUAGES CXX)

find_package(Catch2 3 QUIET REQUIRED)
if (NOT Catch2_FOUND)
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0 # or a later release
    )

    FetchContent_MakeAvailable(Catch2)
endif()
set(CMAKE_CXX_STANDARD 20)

enable_testing()
include(CTest)
include(Catch)

add_executable(TomatoTests)

target_sources(TomatoTests PRIVATE
    ./src/test.cpp
    ./src/moveOperatorTests.cpp
    ./src/variablesTests.cpp
)

target_link_libraries(TomatoTests
    PRIVATE tomato Catch2::Catch2WithMain
)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/tests)

catch_discover_tests(TomatoTests)