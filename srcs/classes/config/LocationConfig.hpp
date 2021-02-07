//
// Created by Louis LAURENT on 02/02/2021.
//

#ifndef WEBSERV_LOCATIONCONFIG_HPP
#define WEBSERV_LOCATIONCONFIG_HPP

#include <ostream>
#include "../../../includes/includes.h"
#define DEFAULT_INDEX "index.html"

class LocationConfig
{
private:
	std::map<std::string, std::string> configuration;

public:

	LocationConfig();

	LocationConfig(const LocationConfig &copy);

	LocationConfig &operator=(const LocationConfig &copy);

	friend std::ostream &operator<<(std::ostream &os, LocationConfig &config);

	std::vector<std::string> getMethods();

	std::vector<std::string> explode(const std::string &s, const std::string &charset);

	std::string getRootDir();

	std::string getPath();

	std::string getAutoindex();

	std::string getUploadDir();

	std::string getDefaultDir();

	std::string getExtension();

	std::string getCgiPath();

	std::string getRawMethods ();

	std::string getIndex ();

	std::map<std::string, std::string> &getConfiguration ();

};


#endif //WEBSERV_LOCATIONCONFIG_HPP
