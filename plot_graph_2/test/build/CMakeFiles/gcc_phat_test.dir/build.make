# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yunseo/sound_custom_test/plot_graph_2/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yunseo/sound_custom_test/plot_graph_2/test/build

# Include any dependencies generated for this target.
include CMakeFiles/gcc_phat_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gcc_phat_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gcc_phat_test.dir/flags.make

CMakeFiles/gcc_phat_test.dir/theory.cpp.o: CMakeFiles/gcc_phat_test.dir/flags.make
CMakeFiles/gcc_phat_test.dir/theory.cpp.o: ../theory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yunseo/sound_custom_test/plot_graph_2/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gcc_phat_test.dir/theory.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gcc_phat_test.dir/theory.cpp.o -c /home/yunseo/sound_custom_test/plot_graph_2/test/theory.cpp

CMakeFiles/gcc_phat_test.dir/theory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gcc_phat_test.dir/theory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yunseo/sound_custom_test/plot_graph_2/test/theory.cpp > CMakeFiles/gcc_phat_test.dir/theory.cpp.i

CMakeFiles/gcc_phat_test.dir/theory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gcc_phat_test.dir/theory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yunseo/sound_custom_test/plot_graph_2/test/theory.cpp -o CMakeFiles/gcc_phat_test.dir/theory.cpp.s

# Object files for target gcc_phat_test
gcc_phat_test_OBJECTS = \
"CMakeFiles/gcc_phat_test.dir/theory.cpp.o"

# External object files for target gcc_phat_test
gcc_phat_test_EXTERNAL_OBJECTS =

gcc_phat_test: CMakeFiles/gcc_phat_test.dir/theory.cpp.o
gcc_phat_test: CMakeFiles/gcc_phat_test.dir/build.make
gcc_phat_test: CMakeFiles/gcc_phat_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yunseo/sound_custom_test/plot_graph_2/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gcc_phat_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gcc_phat_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gcc_phat_test.dir/build: gcc_phat_test

.PHONY : CMakeFiles/gcc_phat_test.dir/build

CMakeFiles/gcc_phat_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gcc_phat_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gcc_phat_test.dir/clean

CMakeFiles/gcc_phat_test.dir/depend:
	cd /home/yunseo/sound_custom_test/plot_graph_2/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yunseo/sound_custom_test/plot_graph_2/test /home/yunseo/sound_custom_test/plot_graph_2/test /home/yunseo/sound_custom_test/plot_graph_2/test/build /home/yunseo/sound_custom_test/plot_graph_2/test/build /home/yunseo/sound_custom_test/plot_graph_2/test/build/CMakeFiles/gcc_phat_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gcc_phat_test.dir/depend

