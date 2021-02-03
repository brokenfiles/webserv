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

int ServerManager::setup_multiple_socket(Config &conf)
{
    std::list<Server*>::iterator it_serv = servers.begin();

    for (std::vector<ServerConfig>::iterator it = conf.getServers().begin(); it != conf.getServers().end(); it++)
    {
        it_serv = servers.insert(it_serv, new Server());
        (*it_serv)->setServerConfig((*it));

        if (((*it_serv)->getServerSocket() = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            return (logger.error("[SERVER]: socket: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        if (fcntl((*it_serv)->getServerSocket(), F_SETFL, O_NONBLOCK) < 0)
            return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        memset(&(*it_serv)->getServerAddr(), 0, sizeof(struct sockaddr_in));
        (*it_serv)->getServerAddr().sin_family = AF_INET;
        (*it_serv)->getServerAddr().sin_port = htons((*it).getPort());
        (*it_serv)->getServerAddr().sin_addr.s_addr = inet_addr((*it).getHost().c_str());

        if (bind((*it_serv)->getServerSocket(), (const struct sockaddr *) &(*it_serv)->getServerAddr(), sizeof((*it_serv)->getServerAddr())) < 0)
            return (logger.error("[SERVER]: bind: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        int opt = 1;
        if ((setsockopt((*it_serv)->getServerSocket(), SOL_SOCKET, SO_REUSEADDR, (char *) &opt, 0)) == 0)
            return (logger.error("[SERVER]: setsockopt: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        if (listen((*it_serv)->getServerSocket(), 20) < 0)
            return (logger.error("[SERVER]: listen" + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        logger.info("[SERVER]: " + (*it_serv)->getServerConfig().getHost() + ":" + logger.to_string((*it_serv)->getServerConfig().getPort()) + " is now listening", NO_PRINT_CLASS);
    }
    return (0);
}