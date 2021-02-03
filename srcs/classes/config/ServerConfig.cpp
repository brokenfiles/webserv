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

int ServerConfig::getPort()
{
	if (this->configuration.find("listen") != this->configuration.end()) {
		return (std::atoi(this->configuration["listen"].c_str()));
	}
	return (DEFAULT_SERVER_PORT);
}

int ServerConfig::getMaxBodySize()
{
	if (this->configuration.find("max_body_size") != this->configuration.end()) {
		return (std::atoi(this->configuration["max_body_size"].c_str()));
	}
	return (DEFAULT_MAX_BODY_SIZE);
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
	if (this->configuration.find("error") != this->configuration.end()) {
		return (this->configuration["error"]);
	}
	return (DEFAULT_ERROR_FILE);
}

std::string ServerConfig::getRootDir()
{
	if (this->configuration.find("root") != this->configuration.end()) {
		return (this->configuration["root"]);
	}
	return ("");
}

std::string ServerConfig::getHost()
{
	if (this->configuration.find("host") != this->configuration.end()) {
		return (this->configuration["host"]);
	}
	return (DEFAULT_HOST);
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
