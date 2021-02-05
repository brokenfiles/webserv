#include "Response.hpp"

Response::Response()
{
    std::cout << "Constructor" << std::endl;
}

Response::~Response()
{
    std::cout << "Destructor" << std::endl;
}

Response::Response(const Response &copy)
{
    std::cout << "Copy constructor" << std::endl;
    *this = copy;
}

Response &Response::operator=(const Response &copy)
{
    std::cout << "Assignation operator" << std::endl;
    if (this != &copy)
    {

    }
    return (*this);
}