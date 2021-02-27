
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/ServerManager.hpp"

#include <ios>

Logger logger;

void parseArguments (int ac, char **av, std::string &config)
{
	int index = 1;
	config = DEFAULT_CONF_FILE;
	while (index < ac)
	{
		std::string argument = std::string(av[index]);
		if (argument == "--silent" || argument == "-s") {
			logger.warning("Silent mode set to : STRONG");
			logger.strong_silence_mode(true);
		}
		else if (index + 1 == ac)
			config = argument;
		index ++;
	}
}

int main (int ac, char **av)
{
	Config			config;
	std::string		configFile;
	ServerManager	serverManager;

	parseArguments(ac, av, configFile);
	logger.requestSilentMode();

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
		return (logger.error("[SERVER]: " + std::string(exception.what()), 1));
	}

	return (0);
}