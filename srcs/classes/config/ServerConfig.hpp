//
// Created by Louis LAURENT on 02/02/2021.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP

#include <ostream>
#include "../../../includes/includes.h"
#include "LocationConfig.hpp"

class ServerConfig
{
private:

public:
	std::list<LocationConfig> locations;
	std::map<std::string, std::string> configuration;

	const std::map<std::string, std::string> &getConfiguration() const;

	void setConfiguration(const std::map<std::string, std::string> &config);

	void addLocation(const LocationConfig& config);

	friend std::ostream &operator<<(std::ostream &os, ServerConfig &config);

};


#endif //WEBSERV_SERVERCONFIG_HPP
