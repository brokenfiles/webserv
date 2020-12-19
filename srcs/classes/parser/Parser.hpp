//
// Created by Timothe Lecouvreur on 18/12/2020.
//

#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP
#include "../../../includes/includes.h"
#include "../queries/Query.hpp"
#include <iostream>
#include <map>

class Parser {
private:

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
		if (query.compare(0, 3, "GET") == 0)
			return ("GET");
		else if (query.compare(0, 3, "POST") == 0)
			return ("POST");
		return (NULL);
	}

	//renvoi une map contenant les headers de la requete

	std::map<std::string, std::string>	getHeaders(std::string &query)
	{
		std::map<std::string, std::string>	map;
		std::size_t							r = 0;
		unsigned int 						tmp = 0;


		//tant qu'on est pas au bout de la requete
		//query.erase( query.length() - 1, 1);
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
	Query&	parse(std::string query) throw(std::exception)
	{
		Query	*ret = new Query();
		if (this->_checkFormat(query) == 0)
			throw std::invalid_argument("Bad format");
		ret->setMethod(getMethod(query));
		ret->setPath(getPath(query, ret->getMethod()));
		ret->setHeaders(getHeaders(query));
		ret->setBody(getBody(query));
		return (*ret);
	}

};



#endif //WEBSERV_PARSER_HPP
