#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>

class Response
{
    public:
        Response();
        ~Response();
        Response(const Response &copy);
        Response &operator=(const Response &copy);

};

#endif
