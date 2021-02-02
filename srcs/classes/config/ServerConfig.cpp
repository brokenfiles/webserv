//
// Created by Louis LAURENT on 02/02/2021.
//

#include "ServerConfig.hpp"

const std::map<std::string, std::string> &ServerConfig::getConfiguration() const
{
	return configuration;
}

void ServerConfig::setConfiguration(const std::map<std::string, std::string> &config)
{
	ServerConfig::configuration = config;
}

void ServerConfig::addLocation(const LocationConfig& location)
{
	this->locations.push_back(location);
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
