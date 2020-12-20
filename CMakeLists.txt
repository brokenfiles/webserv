cmake_minimum_required(VERSION 3.15)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(webserv srcs/webserv.cpp srcs/classes/queries/Query.hpp srcs/classes/logger/Logger.hpp includes/includes.h srcs/classes/server/Server.cpp srcs/classes/server/Server.hpp srcs/classes/parser/Parser.hpp srcs/classes/queries/Query.cpp srcs/classes/client/Client.cpp srcs/classes/queries/Query.cpp srcs/classes/client/Client.hpp)
