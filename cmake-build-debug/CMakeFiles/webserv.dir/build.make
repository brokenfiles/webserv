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
CMAKE_SOURCE_DIR = /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/webserv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/webserv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserv.dir/flags.make

CMakeFiles/webserv.dir/srcs/webserv.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/webserv.cpp.o: ../srcs/webserv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserv.dir/srcs/webserv.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/webserv.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp

CMakeFiles/webserv.dir/srcs/webserv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/webserv.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp > CMakeFiles/webserv.dir/srcs/webserv.cpp.i

CMakeFiles/webserv.dir/srcs/webserv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/webserv.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp -o CMakeFiles/webserv.dir/srcs/webserv.cpp.s

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o: ../srcs/classes/server/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp > CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp -o CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o: ../srcs/classes/queries/Query.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp > CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp -o CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o: ../srcs/classes/client/Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp > CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp -o CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s

# Object files for target webserv
webserv_OBJECTS = \
"CMakeFiles/webserv.dir/srcs/webserv.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o"

# External object files for target webserv
webserv_EXTERNAL_OBJECTS =

webserv: CMakeFiles/webserv.dir/srcs/webserv.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o
webserv: CMakeFiles/webserv.dir/build.make
webserv: CMakeFiles/webserv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable webserv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserv.dir/build: webserv

.PHONY : CMakeFiles/webserv.dir/build

CMakeFiles/webserv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserv.dir/clean

CMakeFiles/webserv.dir/depend:
	cd /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug/CMakeFiles/webserv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserv.dir/depend

