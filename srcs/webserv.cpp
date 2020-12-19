
#include "../includes/includes.h"
#include "../srcs/classes/logger/Logger.hpp"
#include "../srcs/classes/server/Server.hpp"
#include "../srcs/classes/parser/Parser.hpp"
#include "../srcs/classes/queries/Query.hpp"

#define TRUE 1
#define FALSE 0

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

    /* ALLOW MULTIPLE CLIENTS CONNECTION*/
    //je l'ai pas mis class server car ça segfault =)//

    int client_socket[30], max_clients = 30, activity, i, sd;
    int max_sd;
    fd_set readfds;

    memset(client_socket, 0, max_clients);

    int master_socket = server.getSocketServer();

    while (TRUE)
    {
        FD_ZERO(&readfds);

        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
            std::cout << "select_error\n";

        if (FD_ISSET(master_socket, &readfds))
        {
            server.accept_request(master_socket);
            server.send_request("Welcome!");
            for (i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = server.getSocketClient();
                    logger.success("[SERVER]: Adding to list of sockets : " + std::to_string(i), NO_PRINT_CLASS);
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                //need optimisation selon type de connexion;
                server.read_request(sd);
                close(sd);
                client_socket[i] = 0;
                logger.info("[SERVER]: Host disconnected. Socket: " + std::to_string(i), NO_PRINT_CLASS);
            }
        }
    }

    return (0);
}