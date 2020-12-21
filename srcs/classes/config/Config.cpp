
#include "Config.hpp"

Config::Config()
{

}

Config::~Config()
{

}

Config &Config::operator=(const Config &config)
{
	(void)config;
	return (*this);
}

Config::Config(const Config &config)
{
	(void)config;
}

void Config::parse(std::string filename)
{
	(void)filename;
}


