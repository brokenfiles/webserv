#include "Parser.hpp"
#include <vector>

Parser::Parser(void) {}

Parser::~Parser(void) {}

int 		Parser::_checkFormat(std::string query)
{
	if (query.length() == 0)
		return (0);
	return (1);
}

std::string 	Parser::getMethod(std::string query)
{
	std::vector<std::string>	methods(15);

	methods.push_back("GET");
	methods.push_back("POST");
	methods.push_back("LINK");
	methods.push_back("UNLINK");
	methods.push_back("PURGE");
	methods.push_back("PUT");
	methods.push_back("PATCH");
	methods.push_back("DELETE");
	methods.push_back("COPY");
	methods.push_back("HEAD");
	methods.push_back("OPTIONS");
	methods.push_back("LOCK");
	methods.push_back("UNLOCK");
	methods.push_back("PROPFIND");
	methods.push_back("VIEW");

	typedef std::vector<std::string>::iterator iterator;
	iterator begin = methods.begin();
	while (begin != methods.end())
	{
		if (*begin == query.substr(0, query.find(" ")))
			return (*begin);
		begin++;
	}
	return (NULL);
}

std::map<std::string, std::string>	Parser::getHeaders(std::string &query)
{
	std::map<std::string, std::string>	map;
	std::size_t							r = 0;
	unsigned int 						tmp = 0;

	while (query.find('\n') != std::string::npos)
	{
		r = query.find('\n');
		if (query.find('\n') == 1)
			break ;
		if (tmp > 0)
			map[query.substr(0, query.find(':'))] = query.substr(query.find(':') + 2, r - 2 - query.find(':'));
		query.erase(0, r + 1);
		tmp++;
	}
	return (map);
}

std::string 	Parser::getPath(std::string query, std::string method)
{
	query.erase(0, method.length() + 1);
	int 	i = query.find(' ');
	return (query.substr(0, i));
}

std::string 	Parser::getBody(std::string query)
{
	query.erase(0, 2);
	return (query);
}

Request	Parser::parse(std::string input_query) throw(std::exception)
{
	Request	kwery;
	if (this->_checkFormat(input_query) == 0)
		throw std::invalid_argument("Bad format");
	kwery.setMethod(getMethod(input_query));
	kwery.setPath(getPath(input_query, kwery.getMethod()));
	kwery.setHeaders(getHeaders(input_query));
	kwery.setBody(getBody(input_query));
	return (kwery);
}