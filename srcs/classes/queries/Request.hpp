#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# include "../../../includes/includes.h"
# include "../parser/Parser.hpp"
#include "Query.hpp"
#include "../config/LocationConfig.hpp"
#include <map>

class Request : public Query
{
private:
	std::string _path;
	std::string _method;
	std::string _queryString;

public:

	Request ();
	Request (std::string &req);
	~Request ();
	Request &operator= (const Request &copy);

	//getters
	const std::string &getMethod () const;
	const std::string &getPath () const;
	const std::string &getQueryString () const;
	std::string getDefaultPath (LocationConfig &location);

	//setters
	void setPath (const std::string &path);
	void setMethod (const std::string &method);
	void setQueryString (const std::string &queryString);
	static std::string getPathWithoutLocation (const std::string &rawPath, LocationConfig &location);
};
std::ostream &operator<< (std::ostream &o, const Request &q);

#endif
