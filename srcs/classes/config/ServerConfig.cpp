 //
// Created by Louis LAURENT on 02/02/2021.
//

#include "ServerConfig.hpp"

void ServerConfig::addLocation(const LocationConfig& location)
{
	this->locations.push_back(location);
}

ServerConfig &ServerConfig::operator= (const ServerConfig &copy)
{
	this->configuration.clear();
	this->locations.clear();
	this->configuration = copy.configuration;
	this->locations = copy.locations;
	return (*this);
}

int ServerConfig::getPort()
{
	if (this->configuration.find("listen") != this->configuration.end()) {
		return (std::atoi(this->configuration["listen"].c_str()));
	}
	return (DEFAULT_SERVER_PORT);
}

std::string ServerConfig::getServerName()
{
	if (this->configuration.find("server_name") != this->configuration.end()) {
		return (this->configuration["server_name"]);
	}
	return (DEFAULT_SERVER_NAME);
}

std::string ServerConfig::getErrorFile()
{
	if (this->configuration.find("error_template") != this->configuration.end()) {
		return (this->configuration["error_template"]);
	}
	return (DEFAULT_ERROR_FILE);
}

std::string ServerConfig::getHost()
{
	if (this->configuration.find("host") != this->configuration.end()) {
		return (this->configuration["host"]);
	}
	return (DEFAULT_HOST);
}

std::list<LocationConfig> &ServerConfig::getLocations() {
	return (locations);
}

std::ostream &operator<<(std::ostream &os, ServerConfig &server)
{
	std::map<std::string, std::string>::iterator begin = server.configuration.begin();
	while (begin != server.configuration.end()) {
		os << begin->first << " : " << begin->second << std::endl;
		begin++;
	}

	std::list<LocationConfig>::iterator vec_begin = server.locations.begin();
	while (vec_begin != server.locations.end()) {
		os << *vec_begin << std::endl;
		vec_begin++;
	}
	return os;
}
