#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# include "../../../includes/includes.h"
# include "../parser/Parser.hpp"
#include "Query.hpp"
#include <map>

class Request : public Query
{
private:
	std::string 						_path;
	std::string							_method;
	std::string							_queryString;

public:

        Request();
        Request(std::string &req);
        ~Request();
        Request& operator=(const Request& copy);

        //getters
        const std::string &getMethod() const;
        const std::string &getPath() const;
		const std::string &getQueryString() const;

	//setters
        void setPath(const std::string &path);
        void setMethod(const std::string &method);
		void setQueryString(const std::string &queryString);

};

std::ostream&	operator<<(std::ostream &o, const Request &q);

#endif
