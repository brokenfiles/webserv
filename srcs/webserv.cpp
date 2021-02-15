
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

int main (int ac, char **av)
{
	Config        config;
    std::string configFile;
    ServerManager serverManager;
    std::string intput;

	if (ac == 2)
        configFile = av[1];
	else
	    configFile = "srcs/webserv.conf";

	logger.warning("Run Webserv in silent mode? : [y\\n]", NO_PRINT_CLASS);
    std::getline(std::cin, intput);
    bool state = !(intput == "n" || intput == "N");
    logger.notice("SILENT MODE: " + logger.to_string(state) , NO_PRINT_CLASS);
    logger.notice("Loading configuration: " + configFile , NO_PRINT_CLASS);
    logger.silence_mode(state);

	try
	{
		config.parseConfig(configFile);
		config.checkConfig();
	}
	catch (const std::exception &exception)
	{
		std::cerr << exception.what() << std::endl;
		exit(1);
	}

	try
	{
		serverManager.setup_sockets(config);
		serverManager.run_servers();
	}
	catch (const std::exception &exception)
	{
		return (logger.error("[SERVER]: " + logger.to_string(exception.what()), NO_PRINT_CLASS, -1));
	}

	return (0);
}