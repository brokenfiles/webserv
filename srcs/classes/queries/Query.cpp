#include "Query.hpp"
#include "../logger/Logger.hpp"

Query::Query ()
{

}

Query::Query (const Query &query)
{
	*this = query;
}

Query::~Query ()
{

}

Query &Query::operator= (const Query &q)
{
	this->_headers = q.getHeaders();
	this->_body    = q.getBody();
	return (*this);
}

const std::string &Query::getBody () const
{
	return _body;
}

void Query::setHeaders (const std::map<std::string, std::string> &headers)
{
	_headers = headers;
}

void Query::setBody (const std::string& body)
{
	this->_body = body;
	this->_headers["Content-Length"] = Logger::to_string(body.length());
}

const std::map<std::string, std::string> &Query::getHeaders () const
{
	return _headers;
}

std::string Query::getCookies () const
{
	std::string cookieStr = "";

	std::list<std::string>::const_iterator begin = this->_cookies.begin();
	while (begin != this->_cookies.end()) {
		cookieStr += "Set-Cookie: " + *begin + "\n";
		begin++;
	}
	return cookieStr;
}

void Query::addCookie (const std::string &cookie)
{
	this->_cookies.push_back(cookie);
}

void Query::setCookies (std::list<std::string> cookies)
{
	this->_cookies = cookies;
}

std::list<std::string> Query::getRawCookies ()
{
	return this->_cookies;
}
