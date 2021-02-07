//
// Created by Louis LAURENT on 02/02/2021.
//

#include "LocationConfig.hpp"

LocationConfig::LocationConfig ()
{}

LocationConfig::LocationConfig (const LocationConfig &copy)
{
	*this = copy;
}

LocationConfig &LocationConfig::operator= (const LocationConfig &copy)
{
	this->configuration = copy.configuration;
	return (*this);
}

std::vector<std::string> LocationConfig::getMethods()
{
	if (this->configuration.find("methods") != this->configuration.end()) {
		return (explode(this->configuration["methods"], ", "));
	}
	std::vector<std::string> default_methods;
//	default_methods.push_back("get");
	return (default_methods);
}

std::string LocationConfig::getRawMethods()
{
	if (this->configuration.find("methods") != this->configuration.end()) {
		return (this->configuration["methods"]);
	}
	return ("No methods found");
}

std::string LocationConfig::getRootDir() {
	if (this->configuration.find("root") != this->configuration.end()) {
		return (this->configuration["root"]);
	}
	return ("");
}

std::string LocationConfig::getAutoindex() {
	if (this->configuration.find("autoindex") != this->configuration.end()) {
		return (this->configuration["autoindex"]);
	}
	return ("off");
}

std::string LocationConfig::getDefaultDir() {
	if (this->configuration.find("default_dir") != this->configuration.end()) {
		return (this->configuration["default_dir"]);
	}
	return ("");
}

std::string LocationConfig::getUploadDir() {
	if (this->configuration.find("upload_dir") != this->configuration.end()) {
		return (this->configuration["upload_dir"]);
	}
	return ("");
}

std::string LocationConfig::getExtension() {
	if (this->configuration.find("extension") != this->configuration.end()) {
		return (this->configuration["extension"]);
	}
	return ("");
}

std::string LocationConfig::getCgiPath() {
	if (this->configuration.find("cgi_path") != this->configuration.end()) {
		return (this->configuration["cgi_path"]);
	}
	return ("");
}

std::vector<std::string> LocationConfig::explode(const std::string& s, const std::string& charset)
{
	std::string buff = "";
	std::vector<std::string> v;

	for(size_t i = 0; i < s.size(); i++)
	{
		char n = s.at(i);
		if(charset.find(n) == std::string::npos)
			buff+=n;
		else
		if(charset.find(n) != std::string::npos && buff != "") {
			v.push_back(buff);
			buff = "";
		}
	}
	if(buff != "")
		v.push_back(buff);

	return v;
}

std::map<std::string, std::string> &LocationConfig::getConfiguration ()
{
	return configuration;
}

std::ostream &operator<<(std::ostream &os, LocationConfig &location)
{
	os << "Location : " << std::endl;
	std::map<std::string, std::string>::iterator begin = location.configuration.begin();
	while (begin != location.configuration.end()) {
		os << "  " << begin->first << " : " << begin->second << std::endl;
		begin++;
	}
	return os;
}
