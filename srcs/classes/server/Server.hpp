#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "../../../includes/includes.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
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

	//make it works
	int setup(void);
	void setAddress(void);
	int accept_request(void);
	int accept_request(int fd);
	int read_request(void);
	int read_request(int fd);
	int send_request(std::string);
	int close_sock(void);

	int read_request_core(int fd);
	int accept_request_core(int fd);

	int server_run(void);

	//getters
	std::string get_request(void);
	int getSocketServer(void);
	int getSocketClient(void);
	struct sockaddr_in getAddrServer();
	struct sockaddr_in getAddrClient();


private:
	struct sockaddr_in serv_socket_in;
	struct sockaddr_in client_socket_in;
	int                client_sock;
	int                server_sock;
	std::string        request;
	int                client_socket[MAX_CLIENT_HOST];
	int                max_clients;
};

#endif