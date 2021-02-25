#ifndef TEST_SERV_CLIENT_HPP
#define TEST_SERV_CLIENT_HPP

#include "../logger/Logger.hpp"
#include "../../../includes/includes.h"
#include "../queries/Request.hpp"
#include "../config/ServerConfig.hpp"
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

class Parser;

class Client
{
    public:
	    Client();
	    ~Client();
	    Client(const Client &copy);
	    Client &operator=(const Client &copy);

	    int read_request(void);
	    void close_socket();

	    //getters
	    struct sockaddr_in &getAddr();
	    int &getSocket();

	    Request& getObjRequest();


	    std::string &getIP();
	    int &getPort();

	    int &getListener();

	    ServerConfig &getServerConfig();

	    bool &isValidRequest();
	    bool &isConnected();
	    bool &isChunked();
	    bool &isFirstThrough();

	    //setters

	    std::string headerstring;
	    std::string bodystring;


    private:
        Parser parser;
        Request request;

        ServerConfig serverConfig;

	    bool validRequest;
	    bool connected;
	    bool chunk_rep;
        bool firstThrough;

        struct sockaddr_in client_addr;
        int socket;
        int listen;

	    int port;
	    std::string ip;
	    std::string _recvRequest_backup;

};

#endif