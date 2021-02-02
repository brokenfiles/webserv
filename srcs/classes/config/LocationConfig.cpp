//
// Created by Louis LAURENT on 02/02/2021.
//

#include "LocationConfig.hpp"

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
