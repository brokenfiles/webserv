#include "classes/Server.hpp"
#include "includes/includes.h"

int		main(int ac, char **av)
{
    std::cout << "\033[0;31mDon't forget to setup the server connexion properly \033[35;1m[srcs/includes/includes.h]\033[0m\n";
    Server server;

    if (server.setup() == -1)
        return (1);
    if (server.accept_request() == -1)
        return (1);
    if (server.read_request() == -1)
        return (1);
    if (server.send_request() == -1)
        return (1);

    std::cout << server.get_request();

//    file.close();


}