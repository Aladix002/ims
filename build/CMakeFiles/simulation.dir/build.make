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
CMAKE_SOURCE_DIR = /home/aladix/Desktop/FIT/3BIT/ims

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aladix/Desktop/FIT/3BIT/ims/build

# Include any dependencies generated for this target.
include CMakeFiles/simulation.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/simulation.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/simulation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simulation.dir/flags.make

CMakeFiles/simulation.dir/ims.cpp.o: CMakeFiles/simulation.dir/flags.make
CMakeFiles/simulation.dir/ims.cpp.o: ../ims.cpp
CMakeFiles/simulation.dir/ims.cpp.o: CMakeFiles/simulation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aladix/Desktop/FIT/3BIT/ims/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simulation.dir/ims.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/simulation.dir/ims.cpp.o -MF CMakeFiles/simulation.dir/ims.cpp.o.d -o CMakeFiles/simulation.dir/ims.cpp.o -c /home/aladix/Desktop/FIT/3BIT/ims/ims.cpp

CMakeFiles/simulation.dir/ims.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.dir/ims.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aladix/Desktop/FIT/3BIT/ims/ims.cpp > CMakeFiles/simulation.dir/ims.cpp.i

CMakeFiles/simulation.dir/ims.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.dir/ims.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aladix/Desktop/FIT/3BIT/ims/ims.cpp -o CMakeFiles/simulation.dir/ims.cpp.s

# Object files for target simulation
simulation_OBJECTS = \
"CMakeFiles/simulation.dir/ims.cpp.o"

# External object files for target simulation
simulation_EXTERNAL_OBJECTS =

simulation: CMakeFiles/simulation.dir/ims.cpp.o
simulation: CMakeFiles/simulation.dir/build.make
simulation: /usr/local/lib/libsimlib.so
simulation: CMakeFiles/simulation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aladix/Desktop/FIT/3BIT/ims/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simulation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simulation.dir/build: simulation
.PHONY : CMakeFiles/simulation.dir/build

CMakeFiles/simulation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simulation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simulation.dir/clean

CMakeFiles/simulation.dir/depend:
	cd /home/aladix/Desktop/FIT/3BIT/ims/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aladix/Desktop/FIT/3BIT/ims /home/aladix/Desktop/FIT/3BIT/ims /home/aladix/Desktop/FIT/3BIT/ims/build /home/aladix/Desktop/FIT/3BIT/ims/build /home/aladix/Desktop/FIT/3BIT/ims/build/CMakeFiles/simulation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simulation.dir/depend

