//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_QUERY_HPP
#define WEBSERV_QUERY_HPP
#include <iostream>
#include <map>


class Query
{
protected:
	std::map<std::string, std::string>	_headers;
	std::string 						_body;

public:
	Query() {

	}

	virtual ~Query() {}

	Query &operator=(const Query &q) {
		this->_headers = q.getHeaders();
		this->_body = q.getBody();
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

	void setHeaders(const std::map<std::string, std::string> &headers) {
		_headers = headers;
	}

	const std::map<std::string, std::string> &getHeaders() const {
		return _headers;
	}
};

#endif //WEBSERV_QUERY_HPP
