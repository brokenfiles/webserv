
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
	std::list<std::string> lines;

	// Ouverture du fichier de configuration
	std::ifstream file(filename.c_str(), std::ifstream::in);
	// On check si le fichier est bien ouvert
	if (file.good() && file.is_open()) {
		/** Explications scopes_level : dans la configuration, les serveurs sont à la racine donc au niveau 0 */
		int scopes_level = 0, line_index = 0;
		std::list<std::string>::iterator server_begin, server_end;
		// On récupère la ligne
		while ((std::getline(file, line, '\n'))) {
			lines.push_back(line);
			if (line.find('{') != std::string::npos) {
				// si on est au level 0 et qu'on a une scope ouvrante, c'est forcément que c'est le début d'un serveur
				if (scopes_level == 0) {
					server_begin = lines.end();
					server_begin--;
				}
				scopes_level++;
			}
			if (line.find('}') != std::string::npos) {
				scopes_level--;
				if (scopes_level == 0) {
					server_end = lines.end();
					// on a les bounds et les lignes
					this->parseServer(server_begin, server_end);
				}
			}
			line_index ++;
		}
	} else {
		// il y a eu une erreur, throw une exception
		throw std::exception();
	}
}

/*
 * server {
  listen 80;
  host 127.0.0.1;
  server_name  lespelos;
  error      srcs/home/error.html;
  # le root par défaut
  root         srcs/home;
  max_body_size 1000000;

  location / {
    methods GET, POST;
    // le root dans la location / (priorisé)
    root         srcs/home;
    autoindex on;
    default_dir srcs/home;
    upload_dir srcs/uploads;
  }
}
 */

void Config::parseServer(std::list<std::string>::iterator begin, std::list<std::string>::iterator end) {
	ServerConfig server;

	// on parse ligne par ligne le serveur
	while (begin != end) {
		// on sauvegarde dans une const line *begin pour pas avoir besoin de se répéter
		const std::string line = this->removeBeginWhitespaces(*begin);
		if (!line.empty() && line.at(0) != '#') {
			// on est dans une ligne sans commentaire
			if (line.find('{') != std::string::npos) {
				// gérer le scope
			} else {
				// gérer la clé valeur
			}
		}
		begin++;
	}

	this->addServer(server);
}

std::pair<std::string, std::string> getPair(const std::string &line) {
    (void)line;
    return (std::pair<std::string, std::string>(NULL, NULL));
}

void Config::addServer(ServerConfig server)
{
	this->servers.push_back(server);
}

std::vector<ServerConfig> Config::getServers(void)
{
	return std::vector<ServerConfig>();
}

std::string Config::removeBeginWhitespaces(const std::string &line)
{
	// on trouve le premier char qui n'est pas un whitespace
	size_t index = line.find_first_not_of(" \v\t\r");
	if (index == std::string::npos) {
		return (line);
	} else {
		return (line.substr(index, line.size() - index));
	}
}




