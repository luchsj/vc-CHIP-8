# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/queegins/Documents/code/vc-CHIP-8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/queegins/Documents/code/vc-CHIP-8/build

# Include any dependencies generated for this target.
include CMakeFiles/vc-CHIP-8.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/vc-CHIP-8.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vc-CHIP-8.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vc-CHIP-8.dir/flags.make

CMakeFiles/vc-CHIP-8.dir/src/glad.c.o: CMakeFiles/vc-CHIP-8.dir/flags.make
CMakeFiles/vc-CHIP-8.dir/src/glad.c.o: /home/queegins/Documents/code/vc-CHIP-8/src/glad.c
CMakeFiles/vc-CHIP-8.dir/src/glad.c.o: CMakeFiles/vc-CHIP-8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/vc-CHIP-8.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vc-CHIP-8.dir/src/glad.c.o -MF CMakeFiles/vc-CHIP-8.dir/src/glad.c.o.d -o CMakeFiles/vc-CHIP-8.dir/src/glad.c.o -c /home/queegins/Documents/code/vc-CHIP-8/src/glad.c

CMakeFiles/vc-CHIP-8.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/vc-CHIP-8.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/queegins/Documents/code/vc-CHIP-8/src/glad.c > CMakeFiles/vc-CHIP-8.dir/src/glad.c.i

CMakeFiles/vc-CHIP-8.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/vc-CHIP-8.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/queegins/Documents/code/vc-CHIP-8/src/glad.c -o CMakeFiles/vc-CHIP-8.dir/src/glad.c.s

CMakeFiles/vc-CHIP-8.dir/src/main.c.o: CMakeFiles/vc-CHIP-8.dir/flags.make
CMakeFiles/vc-CHIP-8.dir/src/main.c.o: /home/queegins/Documents/code/vc-CHIP-8/src/main.c
CMakeFiles/vc-CHIP-8.dir/src/main.c.o: CMakeFiles/vc-CHIP-8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/vc-CHIP-8.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vc-CHIP-8.dir/src/main.c.o -MF CMakeFiles/vc-CHIP-8.dir/src/main.c.o.d -o CMakeFiles/vc-CHIP-8.dir/src/main.c.o -c /home/queegins/Documents/code/vc-CHIP-8/src/main.c

CMakeFiles/vc-CHIP-8.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/vc-CHIP-8.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/queegins/Documents/code/vc-CHIP-8/src/main.c > CMakeFiles/vc-CHIP-8.dir/src/main.c.i

CMakeFiles/vc-CHIP-8.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/vc-CHIP-8.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/queegins/Documents/code/vc-CHIP-8/src/main.c -o CMakeFiles/vc-CHIP-8.dir/src/main.c.s

CMakeFiles/vc-CHIP-8.dir/src/program.c.o: CMakeFiles/vc-CHIP-8.dir/flags.make
CMakeFiles/vc-CHIP-8.dir/src/program.c.o: /home/queegins/Documents/code/vc-CHIP-8/src/program.c
CMakeFiles/vc-CHIP-8.dir/src/program.c.o: CMakeFiles/vc-CHIP-8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/vc-CHIP-8.dir/src/program.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vc-CHIP-8.dir/src/program.c.o -MF CMakeFiles/vc-CHIP-8.dir/src/program.c.o.d -o CMakeFiles/vc-CHIP-8.dir/src/program.c.o -c /home/queegins/Documents/code/vc-CHIP-8/src/program.c

CMakeFiles/vc-CHIP-8.dir/src/program.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/vc-CHIP-8.dir/src/program.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/queegins/Documents/code/vc-CHIP-8/src/program.c > CMakeFiles/vc-CHIP-8.dir/src/program.c.i

CMakeFiles/vc-CHIP-8.dir/src/program.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/vc-CHIP-8.dir/src/program.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/queegins/Documents/code/vc-CHIP-8/src/program.c -o CMakeFiles/vc-CHIP-8.dir/src/program.c.s

CMakeFiles/vc-CHIP-8.dir/src/wm.c.o: CMakeFiles/vc-CHIP-8.dir/flags.make
CMakeFiles/vc-CHIP-8.dir/src/wm.c.o: /home/queegins/Documents/code/vc-CHIP-8/src/wm.c
CMakeFiles/vc-CHIP-8.dir/src/wm.c.o: CMakeFiles/vc-CHIP-8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/vc-CHIP-8.dir/src/wm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vc-CHIP-8.dir/src/wm.c.o -MF CMakeFiles/vc-CHIP-8.dir/src/wm.c.o.d -o CMakeFiles/vc-CHIP-8.dir/src/wm.c.o -c /home/queegins/Documents/code/vc-CHIP-8/src/wm.c

CMakeFiles/vc-CHIP-8.dir/src/wm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/vc-CHIP-8.dir/src/wm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/queegins/Documents/code/vc-CHIP-8/src/wm.c > CMakeFiles/vc-CHIP-8.dir/src/wm.c.i

CMakeFiles/vc-CHIP-8.dir/src/wm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/vc-CHIP-8.dir/src/wm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/queegins/Documents/code/vc-CHIP-8/src/wm.c -o CMakeFiles/vc-CHIP-8.dir/src/wm.c.s

# Object files for target vc-CHIP-8
vc__CHIP__8_OBJECTS = \
"CMakeFiles/vc-CHIP-8.dir/src/glad.c.o" \
"CMakeFiles/vc-CHIP-8.dir/src/main.c.o" \
"CMakeFiles/vc-CHIP-8.dir/src/program.c.o" \
"CMakeFiles/vc-CHIP-8.dir/src/wm.c.o"

# External object files for target vc-CHIP-8
vc__CHIP__8_EXTERNAL_OBJECTS =

vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/src/glad.c.o
vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/src/main.c.o
vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/src/program.c.o
vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/src/wm.c.o
vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/build.make
vc-CHIP-8: lib/cglm/libcglm.so.0.9.1
vc-CHIP-8: lib/glfw/src/libglfw3.a
vc-CHIP-8: /usr/lib/librt.a
vc-CHIP-8: /usr/lib/libm.so
vc-CHIP-8: CMakeFiles/vc-CHIP-8.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable vc-CHIP-8"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vc-CHIP-8.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vc-CHIP-8.dir/build: vc-CHIP-8
.PHONY : CMakeFiles/vc-CHIP-8.dir/build

CMakeFiles/vc-CHIP-8.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vc-CHIP-8.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vc-CHIP-8.dir/clean

CMakeFiles/vc-CHIP-8.dir/depend:
	cd /home/queegins/Documents/code/vc-CHIP-8/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/queegins/Documents/code/vc-CHIP-8 /home/queegins/Documents/code/vc-CHIP-8 /home/queegins/Documents/code/vc-CHIP-8/build /home/queegins/Documents/code/vc-CHIP-8/build /home/queegins/Documents/code/vc-CHIP-8/build/CMakeFiles/vc-CHIP-8.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/vc-CHIP-8.dir/depend

