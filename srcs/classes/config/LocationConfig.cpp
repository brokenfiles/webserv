//
// Created by Louis LAURENT on 02/02/2021.
//

#include "LocationConfig.hpp"
#include "../queries/Response.hpp"

LocationConfig::LocationConfig ()
{}

LocationConfig::LocationConfig (const LocationConfig &copy)
{
    std::cout << "WESH ALORS\n";
	*this = copy;
}

LocationConfig &LocationConfig::operator= (const LocationConfig &copy)
{
    std::cout << "WESH ALORS!!!!\n";

    this->configuration.clear();
	this->configuration = copy.configuration;
	return (*this);
}

std::vector<std::string> LocationConfig::getMethods()
{
	if (this->configuration.find("methods") != this->configuration.end()) {
		return (explode(this->configuration["methods"], ", ", true));
	}
	return (explode("GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH", ", ", true));
}

std::string LocationConfig::getRawMethods()
{
	if (this->configuration.find("methods") != this->configuration.end()) {
		return (this->configuration["methods"]);
	}
	return ("GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH");
}

std::string LocationConfig::getRootDir() {
	if (this->configuration.find("root") != this->configuration.end()) {
		return (this->configuration["root"]);
	}
	return ("");
}

std::string LocationConfig::getIndex() {
	if (this->configuration.find("index") != this->configuration.end()) {
		return (this->configuration["index"]);
	}
	return (DEFAULT_INDEX);
}

std::string LocationConfig::getAutoindex() {
	if (this->configuration.find("autoindex") != this->configuration.end()) {
		return (this->configuration["autoindex"]);
	}
	return ("off");
}

std::string LocationConfig::getAuth() {
	if (this->configuration.find("auth") != this->configuration.end()) {
		return (this->configuration["auth"]);
	}
	return ("");
}

std::string LocationConfig::getUploadDir() {
	if (this->configuration.find("upload_dir") != this->configuration.end()) {
		return (this->configuration["upload_dir"]);
	}
	return ("");
}

std::string LocationConfig::getCgiExtension() {
	if (this->configuration.find("cgi_extension") != this->configuration.end()) {
		return (this->configuration["cgi_extension"]);
	}
	return ("");
}

int LocationConfig::getMaxBodySize()
{
	if (this->configuration.find("max_body_size") != this->configuration.end()) {
		return (std::atoi(this->configuration["max_body_size"].c_str()));
	}
	return (DEFAULT_MAX_BODY_SIZE);
}

std::string LocationConfig::getCgiBin() {
	if (this->configuration.find("cgi_bin") != this->configuration.end()) {
		return (this->configuration["cgi_bin"]);
	}
	return ("");
}

std::string LocationConfig::getPath ()
{
	if (this->configuration.find("path") != this->configuration.end()) {
		return (this->configuration["path"]);
	}
	return ("/");
}

std::vector<std::string> LocationConfig::explode(const std::string& s, const std::string& charset, bool uppercase = false)
{
	std::string buff = "";
	std::vector<std::string> v;

	for(size_t i = 0; i < s.size(); i++)
	{
		char n = s.at(i);
		if(charset.find(n) == std::string::npos)
			buff+=n;
		else
		if(charset.find(n) != std::string::npos && !buff.empty()) {
			if (uppercase) {
				v.push_back(Utils::toUppercase(buff));
			}
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
