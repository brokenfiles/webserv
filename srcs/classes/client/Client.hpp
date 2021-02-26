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

class Parser;
class Response;

class Client
{
    public:
	    Client();
	    ~Client();
	    Client(const Client &copy);
	    Client &operator=(const Client &copy);

	    int read_request(void);
	    int send_response(std::string &response);
	    void close_socket();
	    void encode_chunk(Response &, std::string &response);
        void clear_state();
        void checkIfIsChunked();

        void printswagresponse(std::string &str);

	    //getters

	    struct sockaddr_in &getAddr();

	    Request& getObjRequest();
        ServerConfig &getServerConfig();
	    Parser& getObjParser();

        std::string &getIP();
        int &getPort();
        int &getSocket();
	    int &getListener();

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