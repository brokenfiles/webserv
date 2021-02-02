
#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "../../../includes/includes.h"
#include "ServerConfig.hpp"

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
	std::vector<ServerConfig> getServers(void);
	std::string removeBeginWhitespaces(const std::string &line);
};

#endif
