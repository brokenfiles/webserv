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

	// TODO: LOUIS: REFACT VECTOR
	methods.push_back("GET");
	methods.push_back("HEAD");
	methods.push_back("POST");
	methods.push_back("PUT");
	methods.push_back("DELETE");
	methods.push_back("CONNECT");
	methods.push_back("OPTIONS");
	methods.push_back("TRACE");
	methods.push_back("PATCH");

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
	int 	i = 0;

	query.erase(0, method.length() + 1);
	i = query.find(' ');
	return (query.substr(0, i));
}

std::string 	Parser::getBody(std::string query, Request request)
{
	query.erase(0, 2);
	if (request.getMethod() == "POST")// && request.getHeaders().find("Content-Type")->second == "application/x-www-form-urlencoded")
		request.setQueryString(query);
	else
		request.setQueryString("");
	return (query);
}

Request	Parser::parse(std::string input_query) throw(std::exception)
{
	Request			request;
	size_t			nb = 0;

	if (this->_checkFormat(input_query) == 0)
		throw std::invalid_argument("Bad format");
	request.setMethod(getMethod(input_query));
	request.setPath(getPath(input_query, request.getMethod()));
	if (request.getMethod() == "GET")
	{
		nb = request.getPath().find('?', 0);
		if (nb != std::string::npos)
			request.setQueryString(request.getPath().substr(nb + 1, request.getPath().length() - nb - 1));
		request.setPath(request.getPath().substr(0, nb));
	}
	request.setHeaders(getHeaders(input_query));
	request.setBody(getBody(input_query, request));
	return (request);
}