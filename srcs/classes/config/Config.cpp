
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

void Config::parseConfig(const std::string &filename)
{
	std::string line;

	// Ouverture du fichier de configuration
	std::ifstream file(filename.c_str(), std::ifstream::in);
	// On check si le fichier est bien ouvert
	if (file.good() && file.is_open()) {
		int scopes = 0, count = 0;
		// On récupère la ligne
		while ((std::getline(file, line, '\n'))) {
			count ++;
			if (line.find('{') != std::string::npos) {
				if (scopes == 0) {
					std::cout << "begin server scope: " << count << std::endl;
				}
				scopes++;
			}
			if (line.find('}') != std::string::npos) {
				scopes--;
				if (scopes == 0) {
					std::cout << "end server scope: " << count << std::endl;
				}
			}
		}
	} else {
		// il y a eu une erreur, throw une exception
//		throw std::bad_function_call();
	}
}


