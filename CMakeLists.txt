cmake_minimum_required(VERSION 3.15)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(webserv srcs/webserv.cpp srcs/classes/queries/Request.hpp srcs/classes/logger/Logger.hpp includes/includes.h srcs/classes/server/Server.cpp srcs/classes/server/Server.hpp srcs/classes/parser/Parser.hpp srcs/classes/queries/Request.cpp srcs/classes/client/Client.cpp srcs/classes/queries/Request.cpp srcs/classes/client/Client.hpp srcs/classes/config/Config.cpp srcs/classes/config/Config.hpp srcs/classes/queries/Response.cpp srcs/classes/queries/Response.hpp srcs/classes/queries/Query.hpp srcs/classes/queries/Query.cpp)
