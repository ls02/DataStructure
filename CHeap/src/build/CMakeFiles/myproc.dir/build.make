# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /home/ls/cmake/bin/cmake

# The command to remove a file.
RM = /home/ls/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ls/github/DataStruct/CHeap/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ls/github/DataStruct/CHeap/src/build

# Include any dependencies generated for this target.
include CMakeFiles/myproc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/myproc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/myproc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myproc.dir/flags.make

CMakeFiles/myproc.dir/test.c.o: CMakeFiles/myproc.dir/flags.make
CMakeFiles/myproc.dir/test.c.o: /home/ls/github/DataStruct/CHeap/src/test.c
CMakeFiles/myproc.dir/test.c.o: CMakeFiles/myproc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ls/github/DataStruct/CHeap/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/myproc.dir/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/myproc.dir/test.c.o -MF CMakeFiles/myproc.dir/test.c.o.d -o CMakeFiles/myproc.dir/test.c.o -c /home/ls/github/DataStruct/CHeap/src/test.c

CMakeFiles/myproc.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/myproc.dir/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ls/github/DataStruct/CHeap/src/test.c > CMakeFiles/myproc.dir/test.c.i

CMakeFiles/myproc.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/myproc.dir/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ls/github/DataStruct/CHeap/src/test.c -o CMakeFiles/myproc.dir/test.c.s

CMakeFiles/myproc.dir/Heap.c.o: CMakeFiles/myproc.dir/flags.make
CMakeFiles/myproc.dir/Heap.c.o: /home/ls/github/DataStruct/CHeap/src/Heap.c
CMakeFiles/myproc.dir/Heap.c.o: CMakeFiles/myproc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ls/github/DataStruct/CHeap/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/myproc.dir/Heap.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/myproc.dir/Heap.c.o -MF CMakeFiles/myproc.dir/Heap.c.o.d -o CMakeFiles/myproc.dir/Heap.c.o -c /home/ls/github/DataStruct/CHeap/src/Heap.c

CMakeFiles/myproc.dir/Heap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/myproc.dir/Heap.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ls/github/DataStruct/CHeap/src/Heap.c > CMakeFiles/myproc.dir/Heap.c.i

CMakeFiles/myproc.dir/Heap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/myproc.dir/Heap.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ls/github/DataStruct/CHeap/src/Heap.c -o CMakeFiles/myproc.dir/Heap.c.s

# Object files for target myproc
myproc_OBJECTS = \
"CMakeFiles/myproc.dir/test.c.o" \
"CMakeFiles/myproc.dir/Heap.c.o"

# External object files for target myproc
myproc_EXTERNAL_OBJECTS =

myproc: CMakeFiles/myproc.dir/test.c.o
myproc: CMakeFiles/myproc.dir/Heap.c.o
myproc: CMakeFiles/myproc.dir/build.make
myproc: CMakeFiles/myproc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ls/github/DataStruct/CHeap/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable myproc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myproc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myproc.dir/build: myproc
.PHONY : CMakeFiles/myproc.dir/build

CMakeFiles/myproc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myproc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myproc.dir/clean

CMakeFiles/myproc.dir/depend:
	cd /home/ls/github/DataStruct/CHeap/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ls/github/DataStruct/CHeap/src /home/ls/github/DataStruct/CHeap/src /home/ls/github/DataStruct/CHeap/src/build /home/ls/github/DataStruct/CHeap/src/build /home/ls/github/DataStruct/CHeap/src/build/CMakeFiles/myproc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myproc.dir/depend

