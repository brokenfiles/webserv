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
