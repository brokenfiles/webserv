#ifndef WEBSERV_REQUEST_HPP
# define WEBSERV_REQUEST_HPP

# include "../../../includes/includes.h"
#include "Query.hpp"
#include <map>

class Request : public Query
{
private:
	std::string 						_path;
	std::string							_method;

public:
        //getters
        const std::string &getMethod() const;
        const std::string &getPath() const;

        //setters
        void setPath(const std::string &path);
        void setMethod(const std::string &method);
};

std::ostream&	operator<<(std::ostream &o, const Request &q);

#endif
