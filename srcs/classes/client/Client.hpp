#ifndef TEST_SERV_CLIENT_HPP
#define TEST_SERV_CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

class Client
{
    public:
        Client();
        Client(int fd, struct sockaddr_in address);
        ~Client();
        Client(const Client &copy);
        Client &operator=(const Client &copy);

        int getSocket();
        std::string getRequest();
        std::string getIP();
        int getPort();

        void setRequest();

    private:
        int socket;
        std::string ip;
        int port;
        std::string request_send;
        struct sockaddr_in client_addr;

};

#endif
