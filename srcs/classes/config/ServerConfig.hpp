//
// Created by Louis LAURENT on 02/02/2021.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP

#include <ostream>
#include "../../../includes/includes.h"
#include "LocationConfig.hpp"
#define DEFAULT_SERVER_PORT 3000
#define DEFAULT_MAX_BODY_SIZE -1
#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_ERROR_FILE "srcs/home/error.html"

class ServerConfig
{
private:

public:
	std::list<LocationConfig> locations;
	std::map<std::string, std::string> configuration;

	const std::map<std::string, std::string> &getConfiguration() const;

	void setConfiguration(const std::map<std::string, std::string> &config);

	void addLocation(const LocationConfig& config);

	int getPort(void);

	friend std::ostream &operator<<(std::ostream &os, ServerConfig &config);

	int getMaxBodySize();

	std::string getServerName();

	std::string getHost();

	std::string getErrorFile();

	std::string getRootDir();

	std::list<LocationConfig> &getLocations();
};


#endif //WEBSERV_SERVERCONFIG_HPP
