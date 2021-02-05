#include "../../../includes/includes.h"
#include "Request.hpp"


Request::Request()
{

}


Request::Request(std::string &req)
{
    Parser parser;

    *this = parser.parse(req);
}

Request::~Request()
{

}

Request &Request::operator=(const Request& copy)
{
    if (this != &copy)
    {
        this->_path = copy._path;
        this->_method = copy._method;
        this->_queryString = copy._queryString;
    }
    return (*this);
}


const std::string &Request::getMethod() const
{
    return _method;
}

const std::string &Request::getPath() const
{
    return _path;
}

void Request::setMethod(const std::string &method)
{
    _method = method;
}

void Request::setPath(const std::string &path)
{
    _path = path;
}

const std::string &Request::getQueryString() const
{
	return _queryString;
}

void Request::setQueryString(const std::string &queryString)
{
	_queryString = queryString;
}

std::ostream&	operator<<(std::ostream &o, const Request &q) {

	o << q.getMethod() << " [" << q.getPath() << "]";
	return (o);
}

int     Request::getPort() const
{
    int             port = 80;

    try {
        std::string     str = this->_headers.at("Host");
        unsigned int    index = str.find(':', 0);
        if (str != "")
        {
            if (str.find(':', 0) != std::string::npos)
                port = std::atoi(str.substr(index + 1, (str.length() - index - 1)).c_str());
            else
                port = std::atoi(str.c_str());
        }
    }
    catch (const std::out_of_range& oor) {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
    std::cout << port << std::endl;
    return (port);
}

