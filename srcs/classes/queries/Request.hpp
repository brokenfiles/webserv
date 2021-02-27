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

	bool headerFilled;
	bool bodyFilled;

public:

	Request ();
	~Request ();
	Request &operator= (const Request &copy);

	//getters
	const std::string &getMethod () const;
	const std::string &getPath () const;
	const std::string &getQueryString () const;
	std::string getDefaultPath (LocationConfig &location);

	bool &isHeaderParsed();
	bool &isBodyParsed();

	//setters
    void setBody (const std::string& body);
	void setPath (const std::string &path);
	void setMethod (const std::string &method);
	void setQueryString (const std::string &queryString);
	static std::string getPathWithoutLocation (const std::string &rawPath, LocationConfig &location);
	static std::string getPathWithIndex (const std::string &rawPath, LocationConfig &location);
	void appendBody(const std::string &body);
	void setBodyRaw(const std::string &body);
};
std::ostream &operator<< (std::ostream &o, const Request &q);

#endif
