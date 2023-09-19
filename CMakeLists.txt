cmake_minimum_required(VERSION 3.8)
project(6502Emulator)

add_subdirectory(lib/Catch2)

# These tests can use the Catch2-provided main
add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


include(CTest)
include(Catch)
catch_discover_tests(tests)