
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
	Config &operator=(const Config &);
	Config(const Config &);

	void parse(std::string filename);
};

#endif
