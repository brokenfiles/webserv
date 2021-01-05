//
// Created by Timothe Lecouvreur on 18/12/2020.
//

#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP
#include "../../../includes/includes.h"
#include "../queries/Request.hpp"
#include <iostream>
#include <map>

class Parser {
public:

	//fonction qui verifie si le format de la requete reçue est valide

	int 		_checkFormat(std::string query)
	{
		if (query.length() == 0)
			return (0);
		return (1);
	}

	//renvoi la methode utilisé dans la requete (GET, POST, ...)

	std::string 	getMethod(std::string query)
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

	//renvoi une map contenant les headers de la requete

	std::map<std::string, std::string>	getHeaders(std::string &query)
	{
		std::map<std::string, std::string>	map;
		std::size_t							r = 0;
		unsigned int 						tmp = 0;


		//tant qu'on est pas au bout de la requete
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

	std::string 	getPath(std::string query, std::string method)
	{
		query.erase(0, method.length() + 1);
		int 	i = query.find(' ');
		return (query.substr(0, i));
	}

	std::string 	getBody(std::string query)
	{
		return (query);
	}

public:
	Parser() {}
	virtual ~Parser() {}

	//methode qui renvoi une reference sur Query apres avoir effectué le parsing
	Request	parse(std::string input_query) throw(std::exception)
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

};



#endif //WEBSERV_PARSER_HPP
