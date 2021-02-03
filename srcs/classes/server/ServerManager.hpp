#ifndef WEBSERV_SERVERMANAGER_HPP
#define WEBSERV_SERVERMANAGER_HPP

#include <iostream>
#include "../config/Config.hpp"
#include "Server.hpp"
#include <list>

class ServerManager
{
    public:
        ServerManager();
        ~ServerManager();
        ServerManager(const ServerManager &copy);
        ServerManager &operator=(const ServerManager &copy);

        int setup_multiple_socket(Config &conf);
    private:
        std::list<Server*> servers;
};

#endif
