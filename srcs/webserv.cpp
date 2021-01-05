
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/Server.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Request.hpp"
#include "../srcs/classes/client/Client.hpp"

#define TRUE 1
#define FALSE 0

Logger logger;

int main(int ac, char **av)
{
	Server server;
	Parser parser;
	Query query;

	logger.warning("Don't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]", NO_PRINT_CLASS);
	(void) av;
	(void) ac;

	/* SETUP SERVER SOCKET AND LISTENING */
	if (server.setup() == -1)
		return (1);

	/* LOOP MULTIPLE CLIENTS */
	if (server.server_run() == -1)
	    return (1);



    return (0);
}