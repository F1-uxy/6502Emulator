# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cflux/Documents/6502Emulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cflux/Documents/6502Emulator

# Include any dependencies generated for this target.
include tests/CMakeFiles/6502EmulatorTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/6502EmulatorTests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/6502EmulatorTests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/6502EmulatorTests.dir/flags.make

tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o: tests/CMakeFiles/6502EmulatorTests.dir/flags.make
tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o: tests/gTests.cpp
tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o: tests/CMakeFiles/6502EmulatorTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cflux/Documents/6502Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o"
	cd /home/cflux/Documents/6502Emulator/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o -MF CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o.d -o CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o -c /home/cflux/Documents/6502Emulator/tests/gTests.cpp

tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/6502EmulatorTests.dir/gTests.cpp.i"
	cd /home/cflux/Documents/6502Emulator/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cflux/Documents/6502Emulator/tests/gTests.cpp > CMakeFiles/6502EmulatorTests.dir/gTests.cpp.i

tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/6502EmulatorTests.dir/gTests.cpp.s"
	cd /home/cflux/Documents/6502Emulator/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cflux/Documents/6502Emulator/tests/gTests.cpp -o CMakeFiles/6502EmulatorTests.dir/gTests.cpp.s

# Object files for target 6502EmulatorTests
6502EmulatorTests_OBJECTS = \
"CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o"

# External object files for target 6502EmulatorTests
6502EmulatorTests_EXTERNAL_OBJECTS =

bin/6502EmulatorTests: tests/CMakeFiles/6502EmulatorTests.dir/gTests.cpp.o
bin/6502EmulatorTests: tests/CMakeFiles/6502EmulatorTests.dir/build.make
bin/6502EmulatorTests: lib/libgtest_main.a
bin/6502EmulatorTests: lib/libgtest.a
bin/6502EmulatorTests: tests/CMakeFiles/6502EmulatorTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cflux/Documents/6502Emulator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/6502EmulatorTests"
	cd /home/cflux/Documents/6502Emulator/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/6502EmulatorTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/6502EmulatorTests.dir/build: bin/6502EmulatorTests
.PHONY : tests/CMakeFiles/6502EmulatorTests.dir/build

tests/CMakeFiles/6502EmulatorTests.dir/clean:
	cd /home/cflux/Documents/6502Emulator/tests && $(CMAKE_COMMAND) -P CMakeFiles/6502EmulatorTests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/6502EmulatorTests.dir/clean

tests/CMakeFiles/6502EmulatorTests.dir/depend:
	cd /home/cflux/Documents/6502Emulator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cflux/Documents/6502Emulator /home/cflux/Documents/6502Emulator/tests /home/cflux/Documents/6502Emulator /home/cflux/Documents/6502Emulator/tests /home/cflux/Documents/6502Emulator/tests/CMakeFiles/6502EmulatorTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/6502EmulatorTests.dir/depend

