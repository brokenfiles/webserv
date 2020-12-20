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

public:
	Query() {

	}

	virtual ~Query() {}

	Query &operator=(const Query &q) {
		this->_method = q.getMethod();
		this->_headers = q.getHeaders();
		return (*this);
	}

	Query(const Query &query) {
		*this = query;
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


/*	int           higher_fd, client_curr, i = 0;
	int           master_socket             = Server::getSocketServer();
	fd_set        fd_pool;
	std::ifstream ifs("query.txt", std::ios::in);
	std::string   newStr;
	if (ifs.is_open())
		std::getline(ifs, newStr, '\0');
	else
	{
		std::cerr << "cannot open the file" << std::endl;
		return (1);
	}
	ifs.close();*/

#endif
