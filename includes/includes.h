#ifndef INCLUDES_H
#define INCLUDES_H

#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Query.hpp"
#include <iostream>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string.h>
#include "../srcs/classes/server/Server.hpp"
#include <fstream>

extern Logger logger;



#define PORT 8080
#define LOCAL "127.0.0.1"
#define BUFFER 100

#endif