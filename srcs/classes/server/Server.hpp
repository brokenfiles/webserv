#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "../../../includes/includes.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/time.h>
#include <fstream>
#include <ctime>
#include <list>
#include "../logger/Logger.hpp"
#include "../parser/Parser.hpp"
#include "../client/Client.hpp"
#include "../response/Response.hpp"

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

	    //accept
	    int accept_request(void);
	    int accept_request(int fd);
        int accept_request_core(int fd);

        //reading
        int read_request(void);
	    int read_request(int fd);
        int read_request_core(int fd);

        //sending
        int send_request(std::string);
	    int send_request(int fd, std::string);
        int send_request_core(int fd, std::string);

	    int close_sock(void);

        //loop
        int server_run(void);
        int setFD_MAX(fd_set &fd_pool, int master_socket);

	    //getters
	    std::string get_request(void);
	    int getSocketServer(void);
	    int getSocketClient(void);
	    std::string getClientIP(void);
	    int getClientPort(void);
	    struct sockaddr_in getAddrServer();
	    struct sockaddr_in getAddrClient();


    private:
	    struct sockaddr_in serv_socket_in;
	    struct sockaddr_in client_socket_in;
	    int                client_sock;
	    int                server_sock;
	    std::string        request;
	    std::list<Client*> client_settled;
	    int                client_socket[MAX_CLIENT_HOST];
	    int                max_clients;
	    Parser             parser;
};

#endif