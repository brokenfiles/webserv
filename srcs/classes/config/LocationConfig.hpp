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

	std::vector<std::string> getMethods();

	std::vector<std::string> explode(const std::string &s, const std::string &charset);

	std::string getRootDir();

	std::string getAutoindex();

	std::string getUploadDir();

	std::string getDefaultDir();

	std::string getExtension();

	std::string getCgiPath();
};


#endif //WEBSERV_LOCATIONCONFIG_HPP
