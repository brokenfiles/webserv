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
	std::string 						_path;
	std::string 						_body;

public:
	Query() {

	}

	virtual ~Query() {}

	Query &operator=(const Query &q) {
		this->_method = q.getMethod();
		this->_headers = q.getHeaders();
		this->_body = q.getBody();
		this->_path = q.getPath();
		return (*this);
	}

	Query(const Query &query) {
		*this = query;
	}

	const std::string &getBody() const {
		return _body;
	}

	void setBody(const std::string &body) {
		_body = body;
	}

	const std::string &getMethod() const {
		return _method;
	}

	const std::string &getPath() const {
		return _path;
	}

	void setPath(const std::string &path) {
		_path = path;
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
