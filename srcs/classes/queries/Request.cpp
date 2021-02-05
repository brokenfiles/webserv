#include "../../../includes/includes.h"
#include "Request.hpp"


Request::Request() : _path(), _method(), _queryString()
{

}


Request::Request(std::string &req) : _path(), _method(), _queryString()
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

