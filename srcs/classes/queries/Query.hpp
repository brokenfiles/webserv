#ifndef WEBSERV_QUERY_HPP
#define WEBSERV_QUERY_HPP

#include <iostream>
#include <map>
#include <list>

class Query
{
protected:
	std::map<std::string, std::string> _headers;
	std::string                        _body;
	std::list<std::string> _cookies;

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
	void setBody (const std::string &body);

	void addCookie(const std::string &cookie);
	std::string getCookies() const;
	void setCookies(std::list<std::string> cookies);
	std::list<std::string> getRawCookies();
};

#endif //WEBSERV_QUERY_HPP
