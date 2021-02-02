//
// Created by Louis LAURENT on 02/02/2021.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP

#include "../../../includes/includes.h"

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

class ServerConfig
{
private:
//	int port, max_body_size;
//	std::string host, server_name, error_file, root_dir;

	std::map<std::string, std::string> configuration;

public:
	const std::map<std::string, std::string> &getConfiguration() const;

	void setConfiguration(const std::map<std::string, std::string> &config);

};


#endif //WEBSERV_SERVERCONFIG_HPP
