#ifndef TEST_SERV_CLIENT_HPP
#define TEST_SERV_CLIENT_HPP

#include "../logger/Logger.hpp"
#include "../../../includes/includes.h"
#include "../queries/Response.hpp"
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

class Client
{
    public:
        Client();
        ~Client();
        Client(const Client &copy);
        Client &operator=(const Client &copy);

        int send_response(const std::string &req);
        int read_request(void);
        void close_socket();
        void printRequest(void);
        void parseRequest(char **env);

        //getters
        struct sockaddr_in &getAddr();
        int &getSocket();
        std::string &getRequest();
        std::string &getIP();
        int &getPort();

        //setters
        void setRequest(std::string& request);

    private:
        struct sockaddr_in client_addr;
        int socket;

        int port;
        std::string ip;
        std::string _recvRequest;
        std::string _parsedRequest;

};

#endif
