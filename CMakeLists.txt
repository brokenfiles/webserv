cmake_minimum_required(VERSION 3.15)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++98")

add_executable(webserv srcs/main.cpp srcs/classes/Server.cpp srcs/classes/Server.hpp)
