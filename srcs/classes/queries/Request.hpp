#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP
# include "../../../includes/includes.h"
#include "Query.hpp"
#include <map>

class Request : public Query
{
private:
	std::string 						_path;
	std::string							_method;

public:
	const std::string &getMethod() const {
		return _method;
	}

	void setMethod(const std::string &method)
    {
	    _method = method;
    }

	const std::string &getPath() const {
		return _path;
	}

	void setPath(const std::string &path) {
		_path = path;
	}
};

std::ostream&	operator<<(std::ostream &o, const Request &q);

#endif
