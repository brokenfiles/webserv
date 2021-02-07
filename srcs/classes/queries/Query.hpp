#ifndef WEBSERV_QUERY_HPP
#define WEBSERV_QUERY_HPP

#include <iostream>
#include <map>

class Query
{
protected:
	std::map<std::string, std::string> _headers;
	std::string                        _body;

public:
	Query ();
	Query (const Query &query);
	virtual ~Query ();
	Query &operator= (const Query &q);
	//getters
	const std::string &getBody () const;
	const std::map<std::string, std::string> &getHeaders () const;
	//setters
	void setHeaders (const std::map<std::string, std::string> &headers);
	void setBody (const std::string& body);
};

#endif //WEBSERV_QUERY_HPP
