project (TomatoTests LANGUAGES CXX)

add_executable(TomatoTests test.cpp)
target_link_libraries(TomatoTests PRIVATE tomato Catch2::Catch2WithMain)

catch_discover_tests(TomatoTests)