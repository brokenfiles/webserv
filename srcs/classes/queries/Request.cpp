#include "../../../includes/includes.h"
#include "Request.hpp"


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

	//std::map<std::string, std::string>::const_iterator	ite;

	o << q.getMethod() << " [" << q.getPath() << "]";
//	for(ite = q.getHeaders().begin(); ite != q.getHeaders().end(); ite++)
//	{
//			o << ite->first << ": ";
//			o << ite->second << std::endl;
//	}
//	o << q.getBody();
	return (o);
}

