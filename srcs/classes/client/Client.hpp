#ifndef TEST_SERV_CLIENT_HPP
#define TEST_SERV_CLIENT_HPP

#include "../logger/Logger.hpp"
#include "../../../includes/includes.h"
#include "../queries/Request.hpp"
#include "../config/ServerConfig.hpp"
#include "../queries/Response.hpp"
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>

class Response;
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
        Response& getObjResponse();


	    std::string &getIP();
	    int &getPort();

	    int &getListener();

	    ServerConfig &getServerConfig();

	    bool &isValidRequest();
	    bool &isConnected();
	    bool &isChunked();
	    bool &isFirstThrough();

	    //setters

    private:
        Parser parser;
        Request request;
        Response response;

        ServerConfig serverConfig;

	    bool validRequest;
	    bool connected;
	    bool chunk_rep;

        struct sockaddr_in client_addr;
        int socket;
        int listen;

        bool firstThrough;

	    int port;
	    std::string ip;
	    std::string _recvRequest_backup;

};

#endif