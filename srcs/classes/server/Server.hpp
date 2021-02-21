#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "../client/Client.hpp"
#include "../config/Config.hpp"
#include "../logger/Logger.hpp"
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

class Client;

class Server
{
    public:
        Server();
        ~Server();
        Server(const Server &copy);
        Server &operator=(const Server &copy);

        int create_socket();
        int accept_client(Client *client);

        int &getServerSocket(void);
        struct sockaddr_in &getServerAddr();
        void setServerConfig(ServerConfig &);
        ServerConfig& getServerConfig(void);

    private:
        ServerConfig        serverConfig;
        struct sockaddr_in  serv_socket_in;
        int                 server_sock;
};

#endif
