cmake_minimum_required(VERSION 3.15)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(webserv srcs/webserv.cpp srcs/classes/queries/Query.hpp)
