#include "ServerManager.hpp"

ServerManager::ServerManager()
{
    FD_ZERO(&this->read_pool);
    FD_ZERO(&this->write_pool);
    FD_ZERO(&this->read_backup);
    FD_ZERO(&this->write_backup);
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

        logger.info("[SERVER]: " + server_curr->getServerConfig().getHost() +" listen on port " + logger.to_string(server_curr->getServerConfig().getPort()) + ".", NO_PRINT_CLASS);
        FD_SET(server_curr->getServerSocket(), &this->read_backup);
        fd_av.push_back(server_curr->getServerSocket());
    }
    std::cout << "\n";
    return (0);
}

int ServerManager::setup_fd()
{
    int higher_fd = -1;
    FD_ZERO(&this->read_pool);
    FD_ZERO(&this->write_pool);
    std::vector<int> read_stack;
    std::vector<int> write_stack;

    std::stringstream stream;

    for (std::list<int>::iterator it = fd_av.begin(); it != fd_av.end(); it++)
    {
        if (FD_ISSET(*it, &this->read_backup))
        {
            FD_SET(*it, &this->read_pool);
            read_stack.push_back(*it);
        }

        if (FD_ISSET(*it, &this->write_backup))
        {
            FD_SET(*it, &this->write_pool);
            write_stack.push_back(*it);
        }
        if (*it > higher_fd)
            higher_fd = *it;
    }

    stream << "FD SERVER SOCKET [";
    for (it_t serv = servers.begin(); serv != servers.end(); serv++)
    {
        if (!(serv._M_node->_M_next == servers.end()._M_node))
            stream << (*serv)->getServerSocket() << ", ";
        else
            stream << (*serv)->getServerSocket();
    }
    logger.notice(stream.str() + "]", NO_PRINT_CLASS);

    stream.str("");

    stream << "FD CLIENT READ_POOL [";
    for (size_t i = 0; i < read_stack.size(); i++)
    {
        if (!((i + 1) == read_stack.size()))
            stream << read_stack[i] << ", ";
        else
            stream << read_stack[i];
    }
    logger.notice(stream.str() + "]", NO_PRINT_CLASS);

    stream.str("");

    stream << "FD CLIENT WRITE_POOL [";
    for (size_t i = 0; i < write_stack.size(); i++)
    {
        if (!((i + 1) == write_stack.size()))
            stream << write_stack[i] << ", ";
        else
            stream << write_stack[i];
    }
    logger.notice(stream.str() + "]", NO_PRINT_CLASS);

    return (higher_fd);
}

int ServerManager::run_servers()
{
    int higher_fd = -1;
    fd_set fd_pool;

    while (1)
    {
        higher_fd = this->setup_fd();

        if (select(higher_fd + 1, &this->read_pool, &this->write_pool, NULL, NULL) < 0)
            return (logger.error("[SERVER]: select: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
        logger.info("[SERVER]: ~ NEW SELECT CALL ~", NO_PRINT_CLASS);

        for (ServerManager::it_t serv_it = this->servers.begin(); serv_it != this->servers.end(); serv_it++)
        {
            Server *server_curr = (*serv_it);
            if (FD_ISSET(server_curr->getServerSocket(), &this->read_pool))
            {
                Client *newClient = new Client();
                newClient->getServerConfig() = server_curr->getServerConfig();

                if (server_curr->accept_client(newClient, fd_pool, higher_fd) < 0)
                    throw AcceptClientError();

                FD_SET(newClient->getSocket(), &this->read_backup);
                fd_av.push_back(newClient->getSocket());

                clients.push_front(newClient);
                logger.connect("[SERVER]: New Client: " + logger.to_string(newClient->getSocket()) + ". Server: " + server_curr->getServerConfig().getHost() + ":" + logger.to_string(server_curr->getServerConfig().getPort()), NO_PRINT_CLASS);
            }
        }

        for (std::list<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            Client *client_curr = (*it);

            if (FD_ISSET(client_curr->getSocket(), &this->read_pool))
            {
                if (client_curr->read_request() < 0)
                {
                    FD_CLR(client_curr->getSocket(), &this->read_backup);
                    FD_CLR(client_curr->getSocket(), &this->read_pool);
                    fd_av.remove(client_curr->getSocket());

                    client_curr->close_socket();
                    it = clients.erase(it);
                    logger.warning(std::string("[SERVER]: Disconnecting from client socket: ") + logger.to_string(client_curr->getSocket()), NO_PRINT_CLASS);
                    continue;
                }

                if (client_curr->isValidRequest())
                    FD_SET(client_curr->getSocket(), &this->write_backup);
            }
            else
                logger.notice(std::string("[SERVER]: Client Loop: FD_ISSET - read_pool: socket " + logger.to_string(client_curr->getSocket()) + " is not set"), NO_PRINT_CLASS);

            if (FD_ISSET(client_curr->getSocket(), &this->write_pool))
            {
                if (client_curr->isValidRequest() && client_curr->isAvailable())
                {
                    Request req(client_curr->getStringRequest());
                    Response rep;

                    client_curr->getObjRequest() = req;

                    std::string response = rep.sendResponse(client_curr);
                    if (send(client_curr->getSocket(), response.c_str(), response.length(), 0) != (int) response.length())
                        return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
                    logger.success("[SERVER]: Client : " + logger.to_string(client_curr->getSocket()) +     ". Response send: file: " + req.getPath(), NO_PRINT_CLASS);
                }
                client_curr->isValidRequest() = false;
                FD_CLR(client_curr->getSocket(), &this->write_backup);
            }
            else
                logger.notice(std::string("[SERVER]: Client Loop: FD_ISSET - write_pool: socket " + logger.to_string(client_curr->getSocket()) + " is not set"), NO_PRINT_CLASS);
        }
    }
}

std::list<Server *> &ServerManager::getServerList()
{
    return (this->servers);
}