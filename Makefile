# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /usr/local/Cellar/cmake/2.8.6/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/2.8.6/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/Cellar/cmake/2.8.6/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kyubuns/Code/codevs

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kyubuns/Code/codevs

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/2.8.6/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/2.8.6/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/kyubuns/Code/codevs/CMakeFiles /Users/kyubuns/Code/codevs/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/kyubuns/Code/codevs/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named codevs.out

# Build rule for target.
codevs.out: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 codevs.out
.PHONY : codevs.out

# fast build rule for target.
codevs.out/fast:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/build
.PHONY : codevs.out/fast

src/Loader.o: src/Loader.cpp.o
.PHONY : src/Loader.o

# target to build an object file
src/Loader.cpp.o:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Loader.cpp.o
.PHONY : src/Loader.cpp.o

src/Loader.i: src/Loader.cpp.i
.PHONY : src/Loader.i

# target to preprocess a source file
src/Loader.cpp.i:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Loader.cpp.i
.PHONY : src/Loader.cpp.i

src/Loader.s: src/Loader.cpp.s
.PHONY : src/Loader.s

# target to generate assembly for a file
src/Loader.cpp.s:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Loader.cpp.s
.PHONY : src/Loader.cpp.s

src/Solver.o: src/Solver.cpp.o
.PHONY : src/Solver.o

# target to build an object file
src/Solver.cpp.o:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Solver.cpp.o
.PHONY : src/Solver.cpp.o

src/Solver.i: src/Solver.cpp.i
.PHONY : src/Solver.i

# target to preprocess a source file
src/Solver.cpp.i:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Solver.cpp.i
.PHONY : src/Solver.cpp.i

src/Solver.s: src/Solver.cpp.s
.PHONY : src/Solver.s

# target to generate assembly for a file
src/Solver.cpp.s:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Solver.cpp.s
.PHONY : src/Solver.cpp.s

src/Task.o: src/Task.cpp.o
.PHONY : src/Task.o

# target to build an object file
src/Task.cpp.o:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Task.cpp.o
.PHONY : src/Task.cpp.o

src/Task.i: src/Task.cpp.i
.PHONY : src/Task.i

# target to preprocess a source file
src/Task.cpp.i:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Task.cpp.i
.PHONY : src/Task.cpp.i

src/Task.s: src/Task.cpp.s
.PHONY : src/Task.s

# target to generate assembly for a file
src/Task.cpp.s:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/Task.cpp.s
.PHONY : src/Task.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/codevs.out.dir/build.make CMakeFiles/codevs.out.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... codevs.out"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... src/Loader.o"
	@echo "... src/Loader.i"
	@echo "... src/Loader.s"
	@echo "... src/Solver.o"
	@echo "... src/Solver.i"
	@echo "... src/Solver.s"
	@echo "... src/Task.o"
	@echo "... src/Task.i"
	@echo "... src/Task.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

