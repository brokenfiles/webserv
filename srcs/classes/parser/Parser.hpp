#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include "../../../includes/includes.h"
#include "../queries/Request.hpp"
#include <iostream>
#include <string>
#include <map>

class Request;

class Parser {

	private:
		void fillMethodAndPath(Request &req, std::string strRequest);
        void fillHeaderAndBody(Request &req, std::string strRequest);

	public:
		Parser();
		~Parser();

        Request	parse(std::string strRequest);

        class BadRequestMethod : public std::exception
        {
            public:
                virtual const char* what() const throw()
                {
                    return ("Bad Request Method");
                }
        };
};


#endif
