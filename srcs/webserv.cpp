
#include "../includes/includes.h"
#include "classes/server/Server.hpp"

Logger logger;

int		main(int ac, char **av)
{
	Server server;

	logger.warning("Don't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]", NO_PRINT_CLASS);
	(void)av;
	(void)ac;

	if (server.setup() == -1)
		return (1);
	if (server.accept_request() == -1)
		return (1);
	if (server.read_request() == -1)
		return (1);
	if (server.send_request() == -1)
		return (1);

	std::cout << server.get_request();

	return (0);
//    file.close();


}