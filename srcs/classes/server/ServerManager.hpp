#ifndef WEBSERV_SERVERMANAGER_HPP
#define WEBSERV_SERVERMANAGER_HPP

#include <iostream>
#include "../config/Config.hpp"
#include "../client//Client.hpp"
#include "../queries/Response.hpp"
#include "Server.hpp"
#include <list>

class ServerManager
{
    public:
        typedef std::list<Server*>::iterator it_t;

        ServerManager();
        ~ServerManager();
        ServerManager(const ServerManager &copy);
        ServerManager &operator=(const ServerManager &copy);

        int setup_sockets(Config &conf);
        int setup_fd(fd_set &fd_pool);
        int read_request(Client *client);
        int send_request(int fd, std::string toSend);
        int run_servers(char **env);

        std::list<Server*>& getServerList();

    private:
        std::list<Server*> servers;
        std::list<Client*> clients;
};

#endif
