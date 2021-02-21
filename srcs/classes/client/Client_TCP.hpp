#ifndef WEBSERV_CLIENT_TCP_HPP
#define WEBSERV_CLIENT_TCP_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cerrno>
#include <queue>
#include <list>
#include <sstream>


class Client_TCP
{
    public:
        Client_TCP();
        ~Client_TCP();
        Client_TCP(const Client_TCP &copy);
        Client_TCP &operator=(const Client_TCP &copy);

        void connectToServer(std::string &host, int port, int size);
        void sendHeader();
        void sendChunkedData();

        struct hostent *host;
        struct sockaddr_in serv_addr;

        struct item {
            int socket;
            struct hostent *serv_h;
            struct sockaddr_in addr;
        };

        std::list<struct item*> clientList;

        int sock;
};

#endif
