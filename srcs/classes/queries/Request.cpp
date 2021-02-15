#include "../../../includes/includes.h"
#include "Request.hpp"
#include "../config/LocationConfig.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
        this->_body = copy._body;
        this->_headers = copy._headers;
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

std::string Request::getDefaultPath(LocationConfig &location)
{
	std::string rootDir = location.getRootDir();
	std::string rawPath = getPathWithoutLocation(_path, location);

	std::string path = rootDir + rawPath;
	struct stat path_stat;

	stat(path.c_str(), &path_stat);
	if (S_ISDIR(path_stat.st_mode)) {
		if (path[path.size() - 1] == '/')
			path += location.getIndex();
		else
			path += "/" + location.getIndex();
	}
	return path;
}

std::string Request::getPathWithoutLocation(const std::string &rawPath, LocationConfig &location) {
	std::string path = rawPath;

	if (location.getPath() != "/") {
		path.erase(path.find(location.getPath()), location.getPath().size());
	}
	return (path);
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

