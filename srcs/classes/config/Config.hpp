
#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "../../../includes/includes.h"
#include "ServerConfig.hpp"
#include "../queries/Request.hpp"

class Config
{
private:
	std::vector<ServerConfig> servers;

public:
	Config();

	virtual ~Config();

	Config &operator=(const Config &config);

	Config(const Config &config);

	void parseConfig(const std::string &filename);

	void parseServer(std::list<std::string>::iterator begin, std::list<std::string>::iterator end);

	void addServer(ServerConfig server);

	void checkConfig();

	ServerConfig &getServer(Request request);

	std::vector<ServerConfig> &getServers(void);

	std::string removeBeginWhitespaces(const std::string &line);

	std::vector<std::string> explode(const std::string &s, const char &c);

	std::pair<std::string, std::string> getPair(const std::string &line);

	class NoSemicolonException : public std::exception
	{
		virtual const char *what() const throw();
	};

	class ScopeException : public std::exception
	{
		virtual const char *what() const throw();
	};

	class MissingFieldException : public std::exception
	{
		virtual const char *what() const throw();
	};

	class MissingLocationException : public std::exception
	{
		virtual const char *what() const throw();
	};
};

#endif
