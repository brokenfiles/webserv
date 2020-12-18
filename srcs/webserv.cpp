
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/Server.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Query.hpp"

Logger logger;

int main(int ac, char **av)
{
	Server server;
	Parser parser;
	Query query;

	logger.warning("Don't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]",
				   NO_PRINT_CLASS);
	(void) av;
	(void) ac;

	if (server.setup() == -1)
		return (1);
	if (server.accept_request() == -1)
		return (1);
	if (server.read_request() == -1)
		return (1);


//	try {		// appel de la fonction de parsing
//		query = parser.parse(server.get_request());
//
//		affich le contenue de la requete qui vient d'être lue
//		std::cout << query << std::endl;
//	}
//	catch (std::exception &e) {		//en cas d'erreur utiliser le logger
//		logger.error("Bad format in query", NO_PRINT_CLASS);
//	}


	if (server.send_request() == -1)
		return (1);

	//std::cout << server.get_request();

	return (0);
//    file.close();


}