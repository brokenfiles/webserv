
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/Server.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Request.hpp"
#include "../srcs/classes/client/Client.hpp"
#include "classes/config/Config.hpp"

#define TRUE 1
#define FALSE 0

Logger logger;

int main(int ac, char **av, char **envp)
{
	Config config;
	Server server;
	Parser parser;
	Query query;


	try {
		config.parseConfig("srcs/webserv.conf");
		config.checkConfig();
	} catch (const std::exception &exception) {
		std::cerr << exception.what() << std::endl;
		exit(1);
	}

	std::cout << config.getServers()[0].getPort() << std::endl;

//	exit(0);

	logger.warning("Don't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]", NO_PRINT_CLASS);
	(void) av;
	(void) ac;
	(void) envp;

	std::list<Server*> servers;
	/* SET SERVER CONFIGS */
	server.setConfig(config);
	server.setup_multiple_socket(servers);


	/* SETUP SERVER SOCKET AND LISTENING */
//	if (server.setup() == -1)
//		return (1);

	/* LOOP MULTIPLE CLIENTS */
//	if (server.server_run(envp) == -1)
//	    return (1);

    return (0);
}