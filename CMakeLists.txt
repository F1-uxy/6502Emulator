cmake_minimum_required(VERSION 3.8)
project(6502Emulator)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(SOURCES 
    "src/6502main.cpp"
    "src/CPU.cpp"
    "src/Memory.cpp"
    "src/Loader.cpp"
    )

add_executable(6502Emulator ${SOURCES})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)

