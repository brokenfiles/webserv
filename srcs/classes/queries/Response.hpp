#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include "../client/Client.hpp"

class Client;

class Response
{
    public:
        Response();
        ~Response();
        Response(const Response &copy);
        Response &operator=(const Response &copy);

        std::string sendResponse(Client *client);
};

#endif
