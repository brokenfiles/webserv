
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/ServerManager.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Request.hpp"
#include "../srcs/classes/client/Client.hpp"
#include "classes/config/Config.hpp"

#define TRUE 1
#define FALSE 0

Logger logger;

int main(int ac, char **av, char **env)
{
	Config config;
	Parser parser;
	Query query;
    ServerManager serverManager;

	try {
		config.parseConfig("srcs/webserv.conf");
		config.checkConfig();
	} catch (const std::exception &exception) {
		std::cerr << exception.what() << std::endl;
		exit(1);
	}

	std::cout << config.getServers()[0].getLocations().front().getMethods().front() << std::endl;

	logger.warning("Don't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]", NO_PRINT_CLASS);
	(void) av;
	(void) ac;

    try
    {
        serverManager.setup_sockets(config);
        serverManager.run_servers(env);
    }
    catch (const std::exception &exception)
    {
        return (logger.error("[SERVER]: " + logger.to_string(exception.what()), NO_PRINT_CLASS, -1));
    }

    return (0);
}