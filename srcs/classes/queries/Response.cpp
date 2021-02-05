#include "Response.hpp"

Response::Response()
{

}

Response::~Response()
{

}

Response::Response(const Response &copy)
{
    *this = copy;
}

Response &Response::operator=(const Response &copy)
{
    if (this != &copy)
    {

    }
    return (*this);
}

std::string Response::sendResponse(Client *client)
{
    (void)client;
    return (std::string("cc"));
}