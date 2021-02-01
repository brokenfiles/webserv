
#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "../server/Server.hpp"

class Config
{
private:
	std::vector<Server> servers;

public:
	Config();
	virtual ~Config();
	Config &operator=(const Config &config);
	Config(const Config &config);

	void parseConfig(const std::string &filename);
};

#endif
