#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP
#include "../../../includes/includes.h"
#include "../queries/Request.hpp"
#include <iostream>
#include <map>

class Parser {
	private:
		//methodes qui recuperent les infos dans une requete
		int 		_checkFormat(std::string query);
		std::string 	getMethod(std::string query);
		std::map<std::string, std::string>	getHeaders(std::string &query);
		std::string 	getPath(std::string query, std::string method);
		std::string 	getBody(std::string query);

	public:
		Parser();
		virtual ~Parser();

		Request	parse(std::string input_query) throw(std::exception);
};



#endif //WEBSERV_PARSER_HPP
