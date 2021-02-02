
#include "Config.hpp"
#include "LocationConfig.hpp"

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

/**
 * Parsing complet de la configuration
 * La fonction ouvre le fichier, check si il est bien ouvert et peut être lu
 * On va lire ligne par ligne si il y a un scope
 * 	ajouter un à scopeLevel si scope ouvrante
 * 	enlever un à scopeLevel si scope fermante
 * Le scopelevel 0 veut dire qu'on est à la racine
 * @param filename
 */
void Config::parseConfig(const std::string &filename)
{
	std::string line;
	std::list<std::string> lines;

	// Ouverture du fichier de configuration
	std::ifstream file(filename, std::ifstream::in);
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
		std::vector<ServerConfig>::iterator begin = servers.begin();
		while(begin != servers.end()) {
			std::cout << *begin << std::endl;
			begin++;
		}
	} else {
		// il y a eu une erreur, throw une exception
		throw std::bad_function_call();
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

/**
 * Parse le serveur ligne par ligne de begin à end
 * Tous les lignes, la fonction check si c'est un commentaire, si c'est le cas on ignore
 * Si ce n'est pas le cas, on regarde si il y a un scope (dans ce cas on est dans une location)
 * Si il n'y a pas de scope, c'est que l'on est dans le cas <clé valeur;> et on appelle getPair
 * @param begin - begin of the line range
 * @param end - end of the line range
 */
void Config::parseServer(std::list<std::string>::iterator begin, std::list<std::string>::iterator end) {
	ServerConfig server;
	int scopeLevel = 0;
	LocationConfig currentLocation = LocationConfig();

	// on parse ligne par ligne le serveur
	while (begin != end) {
		// on sauvegarde dans une const line *begin pour pas avoir besoin de se répéter
		const std::string line = this->removeBeginWhitespaces(*begin);
		if (!line.empty() && line.at(0) != '#') {
			// on est dans une ligne sans commentaire
			if (line.find('{') != std::string::npos || line.find('}') != std::string::npos) {
				// gérer les différentes scopes
				if (line.find('{') != std::string::npos) {
					scopeLevel ++;
					if (line.find("location") != std::string::npos) {
						// get path
						size_t index = line.find(' ');
						if (index != std::string::npos) {
							std::string path = removeBeginWhitespaces(line.substr(index, line.size() - index));
							size_t nextSpace = path.find(' ');
							if (nextSpace != std::string::npos) {
								path = path.substr(0, nextSpace);
								currentLocation.configuration["path"] = path;
							}
						}
					}
				}
				else if (line.find('}') != std::string::npos) {
					scopeLevel --;
					// on est à la fin d'une location
					if (scopeLevel == 1) {
						server.addLocation(currentLocation);
						currentLocation = LocationConfig();
					}
				}
			} else if (scopeLevel == 1 || scopeLevel == 2) {
				// gérer la clé valeur (le scope 0 indique qu'on est dans le serveur)
				try {
					std::pair<std::string, std::string> pair = getPair(line);
					if (scopeLevel == 1)
						server.configuration.insert(pair);
					else
						currentLocation.configuration.insert(pair);
				} catch (const std::exception &e) {
					std::cout << e.what() << std::endl;
					exit(1);
				}
			} else {
				// erreur

			}
		}
		begin++;
	}
	this->addServer(server);
}

/**
 * Récupère dans une std::pair les deux morceaux (clé valeur) pour chaque ligne
 * @param line
 * @return la pair qui contient la clé (first) et la valeur (second)
 */
std::pair<std::string, std::string> Config::getPair(const std::string &line) {
	std::pair<std::string, std::string> pair;
	std::vector<std::string> parts = explode(line, ' ');
	if (parts.size() == 2) {
		// on check si il y a bien le point virgule sinon on throw une erreur
		if (parts[1].at(parts[1].size() - 1) != ';') {
			std::cout << "ligne sans point virgule : " << line << std::endl;
			throw NoSemicolonException();
		}
		// suppression du point virgule
		parts[1].erase(parts[1].size() - 1, 1);
		pair.first = parts[0];
		// pair.second vaut la partie 2 sans les whitespaces du début
		pair.second = removeBeginWhitespaces(parts[1]);
	}
	return pair;
}

/**
 * Enlève les whitespaces avant chaque ligne pour parser plus facilement
 * @param line
 * @return la ligne sans les whitespaces du début
 */
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

/**
 * Cete fonction permet de split une seule fois sur le caractère c
 * @warning cette fonction split une seule fois, dès qu'elle recontre le char c elle arrête de split
 * elle vient de cplusplus mais a été modifiée
 * @param s la string que l'on souhaite splitter
 * @param c le char qui split
 * @return un vector de string qui contient nos morceaux
 */
std::vector<std::string> Config::explode(const std::string& s, const char& c)
{
	std::string buff = "";
	std::vector<std::string> v;
	bool already_split = false;

	for(size_t i = 0; i < s.size(); i++)
	{
		char n = s.at(i);
		if(n != c || already_split)
			buff+=n;
		else
			if(n == c && buff != "") {
				v.push_back(buff);
				buff = "";
				already_split = true;
			}
	}
	if(buff != "")
		v.push_back(buff);

	return v;
}

void Config::addServer(ServerConfig server)
{
	this->servers.push_back(server);
}

std::vector<ServerConfig> Config::getServers(void)
{
	return std::vector<ServerConfig>();
}

const char *Config::NoSemicolonException::what() const throw()
{
	return "Pas de point virgule dans la configuration";
}
