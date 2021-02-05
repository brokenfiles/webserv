#include "ServerManager.hpp"

ServerManager::ServerManager()
{

}

ServerManager::~ServerManager()
{

}

ServerManager::ServerManager(const ServerManager &copy)
{
    *this = copy;
}

ServerManager &ServerManager::operator=(const ServerManager &copy)
{
    if (this != &copy)
    {

    }
    return (*this);
}

int ServerManager::setup_sockets(Config &conf)
{
    std::list<Server*>::iterator it_serv = servers.begin();

    for (std::vector<ServerConfig>::iterator it_conf = conf.getServers().begin(); it_conf != conf.getServers().end(); it_conf++)
    {
        it_serv = servers.insert(it_serv, new Server());
        Server* server_curr = (*it_serv);

        server_curr->setServerConfig((*it_conf));
        if (server_curr->create_socket() < 0)
            throw SetupSocketError();

        logger.info("[SERVER]: " + server_curr->getServerConfig().getHost() + ":" + logger.to_string(server_curr->getServerConfig().getPort()) + " is now listening", NO_PRINT_CLASS);
    }
    return (0);
}

int ServerManager::setup_fd(fd_set &fd_pool)
{
    int higher_fd;

    FD_ZERO(&fd_pool);
    higher_fd = -1;

    for (ServerManager::it_t serv_it = this->servers.begin(); serv_it != this->servers.end(); serv_it++)
    {
        FD_SET((*serv_it)->getServerSocket(), &fd_pool);
        if ((*serv_it)->getServerSocket() > higher_fd)
            higher_fd = (*serv_it)->getServerSocket();
    }
    for (std::list<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        int client_curr = (*it)->getSocket();
        if (client_curr > 0)
            FD_SET(client_curr, &fd_pool);
        if (client_curr > higher_fd)
            higher_fd = client_curr;
    }
    return (higher_fd);
}

int ServerManager::run_servers(char **env)
{
    int higher_fd;
    fd_set fd_pool;

    while (1)
    {
        higher_fd = this->setup_fd(fd_pool);

        if (select(higher_fd + 1, &fd_pool, NULL, NULL, NULL) < 0)
            return (logger.error("[SERVER]: select: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        for (ServerManager::it_t serv_it = this->servers.begin(); serv_it != this->servers.end(); serv_it++)
        {
            Server *server_curr = (*serv_it);
            if (FD_ISSET(server_curr->getServerSocket(), &fd_pool))
            {
                Client *newClient = new Client();
                if (server_curr->accept_client(newClient, fd_pool, higher_fd) < 0)
                    throw AcceptClientError();

                logger.notice("[SERVER]: New Client connexion: " + logger.to_string(newClient->getSocket()),
                              NO_PRINT_CLASS);
                clients.push_front(newClient);
            }
        }

        for (std::list<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            Client *client_curr = (*it);
            Response rep;
            (void) env;

            if (FD_ISSET(client_curr->getSocket(), &fd_pool))
            {
                int ret_code;
                if ((ret_code = client_curr->read_request()) < 0)
                {
                    if (ret_code == -2)
                    {
                        client_curr->close_socket();
                        it = clients.erase(it);
                        logger.notice(std::string("[SERVER]: Empty Request: Ejecting: ") + logger.to_string(client_curr->getSocket()), NO_PRINT_CLASS);
                        continue;
                    }
                    throw ReadClientSocket();
                }

                client_curr->printRequest();

                Request req(client_curr->getStringRequest());
                client_curr->getObjRequest() = req;



                std::string response = rep.sendResponse(client_curr);

                if (send(client_curr->getSocket(), response.c_str(), response.length(), 0) != (int) response.length())
                    return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

                client_curr->close_socket();
                it = clients.erase(it);

                logger.notice(std::string("[SERVER]: Disconnecting from client sock: ") + logger.to_string(client_curr->getSocket()), NO_PRINT_CLASS);
            }
        }
    }
}

std::list<Server *> &ServerManager::getServerList()
{
    return (this->servers);
}