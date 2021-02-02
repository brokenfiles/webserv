//
// Created by Louis LAURENT on 02/02/2021.
//

#ifndef WEBSERV_LOCATIONCONFIG_HPP
#define WEBSERV_LOCATIONCONFIG_HPP

#include <ostream>
#include "../../../includes/includes.h"


class LocationConfig
{
public:
	std::map<std::string, std::string> configuration;

	friend std::ostream &operator<<(std::ostream &os, LocationConfig &config);
};


#endif //WEBSERV_LOCATIONCONFIG_HPP
