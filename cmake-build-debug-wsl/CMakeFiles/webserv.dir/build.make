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
CMAKE_BINARY_DIR = /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl

# Include any dependencies generated for this target.
include CMakeFiles/webserv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/webserv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserv.dir/flags.make

CMakeFiles/webserv.dir/srcs/webserv.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/webserv.cpp.o: ../srcs/webserv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserv.dir/srcs/webserv.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/webserv.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp

CMakeFiles/webserv.dir/srcs/webserv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/webserv.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp > CMakeFiles/webserv.dir/srcs/webserv.cpp.i

CMakeFiles/webserv.dir/srcs/webserv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/webserv.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/webserv.cpp -o CMakeFiles/webserv.dir/srcs/webserv.cpp.s

CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o: ../srcs/classes/queries/Request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Request.cpp

CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Request.cpp > CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.i

CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Request.cpp -o CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.s

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o: ../srcs/classes/client/Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp > CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.i

CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client.cpp -o CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.s

CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o: ../srcs/classes/config/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/Config.cpp

CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/Config.cpp > CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.i

CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/Config.cpp -o CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.s

CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o: ../srcs/classes/queries/Response.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Response.cpp

CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Response.cpp > CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.i

CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Response.cpp -o CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.s

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o: ../srcs/classes/queries/Query.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp > CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.i

CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/queries/Query.cpp -o CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.s

CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o: ../srcs/classes/parser/Parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/parser/Parser.cpp

CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/parser/Parser.cpp > CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.i

CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/parser/Parser.cpp -o CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.s

CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o: ../srcs/classes/cgi/Cgi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/cgi/Cgi.cpp

CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/cgi/Cgi.cpp > CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.i

CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/cgi/Cgi.cpp -o CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.s

CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o: ../srcs/utils/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/utils/utils.cpp

CMakeFiles/webserv.dir/srcs/utils/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/utils/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/utils/utils.cpp > CMakeFiles/webserv.dir/srcs/utils/utils.cpp.i

CMakeFiles/webserv.dir/srcs/utils/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/utils/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/utils/utils.cpp -o CMakeFiles/webserv.dir/srcs/utils/utils.cpp.s

CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o: ../srcs/classes/config/ServerConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/ServerConfig.cpp

CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/ServerConfig.cpp > CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.i

CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/ServerConfig.cpp -o CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.s

CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o: ../srcs/classes/config/LocationConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/LocationConfig.cpp

CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/LocationConfig.cpp > CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.i

CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/config/LocationConfig.cpp -o CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.s

CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o: ../srcs/classes/server/ServerManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/ServerManager.cpp

CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/ServerManager.cpp > CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.i

CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/ServerManager.cpp -o CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.s

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o: ../srcs/classes/server/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp > CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.i

CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/server/Server.cpp -o CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.s

CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o: ../srcs/classes/client/Client_TCP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o -c /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client_TCP.cpp

CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client_TCP.cpp > CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.i

CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/srcs/classes/client/Client_TCP.cpp -o CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.s

# Object files for target webserv
webserv_OBJECTS = \
"CMakeFiles/webserv.dir/srcs/webserv.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o" \
"CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o" \
"CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o"

# External object files for target webserv
webserv_EXTERNAL_OBJECTS =

webserv: CMakeFiles/webserv.dir/srcs/webserv.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/queries/Request.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/client/Client.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/config/Config.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/queries/Response.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/queries/Query.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/parser/Parser.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/cgi/Cgi.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/utils/utils.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/config/ServerConfig.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/config/LocationConfig.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/server/ServerManager.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/server/Server.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/classes/client/Client_TCP.cpp.o
webserv: CMakeFiles/webserv.dir/build.make
webserv: CMakeFiles/webserv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX executable webserv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserv.dir/build: webserv

.PHONY : CMakeFiles/webserv.dir/build

CMakeFiles/webserv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserv.dir/clean

CMakeFiles/webserv.dir/depend:
	cd /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl /mnt/c/Users/m4xdo/OneDrive/Bureau/webserv/cmake-build-debug-wsl/CMakeFiles/webserv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserv.dir/depend

