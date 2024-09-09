# Add a separate build for tests
add_executable(6502EmulatorTests "gTests.cpp")

# Link the test executable with Google Test
target_link_libraries(6502EmulatorTests gtest_main)

# Add the test as a CTest test
add_test(NAME 6502EmulatorTests COMMAND 6502EmulatorTests)

