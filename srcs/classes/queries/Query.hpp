#ifndef WEBSERV_QUERY_HPP
# define WEBSERV_QUERY_HPP
# include "../../../includes/includes.h"
#include <iostream>
#include <map>

class Query
{
private:
	std::string							_method;
	std::map<std::string, std::string>	_headers;

public:
	Query() {}
	~Query() {}

	Query &operator=(const Query &q) {
		this->_method = q.getMethod();
		this->_headers = q.getHeaders();
		return (*this);
	}

	Query(const Query &);

	const std::string &getMethod() const {
		return _method;
	}

	void setMethod(const std::string &method) {
		_method = method;
	}

	void setHeaders(const std::map<std::string, std::string> &headers) {
		_headers = headers;
	}

	const std::map<std::string, std::string> &getHeaders() const {
		return _headers;
	}


};

std::ostream&	operator<<(std::ostream &o, const Query &q);


#endif
