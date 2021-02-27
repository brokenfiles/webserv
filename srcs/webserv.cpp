
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/ServerManager.hpp"

#include <ios>

Logger logger;

int setSilentsModes(int ac, char **argv, std::string &configFile)
{
//    int i = 0;
    if (ac > 2 && ac <= 3)
    {
        if (std::strcmp(argv[1], "--silent") == 0)
        {
            logger.warning("SILENT MODE: STRONG");
            logger.strong_silence_mode(true);
        }
        if (argv[2])
            configFile = argv[2];
    }

    if (ac == 2 || !logger.isStrongSilent())
    {
        std::string input;
        logger.warning("Run Webserv in silent mode? : [y\\n]");
        std::getline(std::cin, input);
        bool state = !(input == "n" || input == "N");
        logger.notice("SILENT MODE: " + logger.to_string(state) );
        logger.silence_mode(state);
        if (ac == 2)
            configFile = argv[1];
    }
    else if (ac < 2)
        configFile = "conf/default.conf";
    return (0);
}

int main (int ac, char **av)
{
	Config        config;
    std::string configFile;
    ServerManager serverManager;

    setSilentsModes(ac, av, configFile);

	try
	{
		config.parseConfig(configFile);
		config.checkConfig();
        logger.success("Loading configuration: " + configFile);
    }
	catch (const std::exception &exception)
	{
	    logger.error("[SERVER]: Config check: " + std::string(exception.what()) + ".");
		exit(1);
	}

	try
	{
		serverManager.setup_sockets(config);
		serverManager.run_servers();
	}
	catch (const std::exception &exception)
	{
		return (logger.error("[SERVER]: " + logger.to_string(exception.what()), -1));
	}

	return (0);
}