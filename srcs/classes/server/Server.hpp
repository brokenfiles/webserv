#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "../../../includes/includes.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <fstream>
#include "../logger/Logger.hpp"

class Server
{
public:
	Server();
	~Server();
	Server(const Server &copy);
	Server &operator=(const Server &copy);
	int setup(void);
	void setAddress(void);
	int accept_request(void);
	int read_request(void);
	int send_request(void);
	int close_sock(void);
	std::string get_request(void);

private:
	struct sockaddr_in serv_socket_in;
	struct sockaddr_in client_socket_in;
	int                client_sock;
	int                server_sock;
	std::string        request;
};

#endif