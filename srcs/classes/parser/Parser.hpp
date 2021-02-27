#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include "../../../includes/includes.h"
#include "../logger/Logger.hpp"
#include "../queries/Query.hpp"
#include "../queries/Request.hpp"
#include <iostream>
#include <string>
#include <map>

class Request;

class Parser
{

private:
	void fillMethod (Request &, std::string &);
	void fillPath (Request &, std::string &);
	void fillHeader (Request &, std::string &);
	void fillQueryString (Request &);

public:
	Parser ();
	~Parser ();
	Request parseResponse (std::string strResponse);

	void parseHeader(Request& req, std::string& keeper);
	void parseBody(Request& req, std::string& keeper);
	int fillChunk(std::string &keeper, Request& parser);
	int fillContentSize(std::string &keeper, std::string strsize);

};

#endif
