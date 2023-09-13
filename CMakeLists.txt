cmake_minimum_required(VERSION 3.25)
project(6502Emulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(6502Emulator 6502main.cpp 6502main.h)
