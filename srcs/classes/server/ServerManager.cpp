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
    for (std::list<Server*>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        delete (*it);
        it = servers.erase(it);
    }

    for (std::list<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        delete (*it);
        it = clients.erase(it);
    }
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
    std::vector<int> port_listening;

    this->set_global_config(conf);

    for (std::vector<ServerConfig>::iterator it_conf = conf.getServers().begin(); it_conf != conf.getServers().end(); it_conf++)
    {
        std::vector<int>::iterator it_ip = std::find(port_listening.begin(), port_listening.end(), (*it_conf).getPort());
        if (it_ip == port_listening.end())
        {
            port_listening.push_back((*it_conf).getPort());
            it_serv = servers.insert(it_serv, new Server());
            Server *server_curr = (*it_serv);

            server_curr->setServerConfig((*it_conf));
            if (server_curr->create_socket() < 0)
                throw SetupSocketError();

            logger.info("[SERVER]: " + server_curr->getServerConfig().getHost() + " listen on port " +
                        logger.to_string(server_curr->getServerConfig().getPort()) + ".");
            FD_SET(server_curr->getServerSocket(), &this->read_backup);
            fd_av.push_back(server_curr->getServerSocket());
        }
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
		if (serv != servers.end()) {
            stream << (*serv)->getServerSocket() << ", ";
		} else {
			stream << (*serv)->getServerSocket();
		}
    }
    logger.notice(stream.str() + "]");

    stream.str("");

    stream << "FD CLIENT READ_POOL [";
    for (size_t i = 0; i < read_stack.size(); i++)
    {
        if ((i + 1) != read_stack.size())
            stream << read_stack[i] << ", ";
        else
            stream << read_stack[i];
    }
    logger.notice(stream.str() + "]");

    stream.str("");

    stream << "FD CLIENT WRITE_POOL [";
    for (size_t i = 0; i < write_stack.size(); i++)
    {
        if ((i + 1) != write_stack.size())
            stream << write_stack[i] << ", ";
        else
            stream << write_stack[i];
    }
    logger.notice(stream.str() + "]");

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
            return (logger.error("[SERVER]: select: " + std::string(strerror(errno)), -1));
        logger.info("[SERVER]: ~ NEW SELECT CALL ~");

        for (ServerManager::it_t serv_it = this->servers.begin(); serv_it != this->servers.end(); serv_it++)
        {
            Server *server_curr = (*serv_it);
            if (FD_ISSET(server_curr->getServerSocket(), &this->read_pool))
            {
                Client *newClient = new Client();

                newClient->getListener() = server_curr->getServerConfig().getPort();

                if (server_curr->accept_client(newClient, fd_pool, higher_fd) < 0)
                    throw AcceptClientError();

                if (this->fd_av.size() > 916)
                {
                    newClient->isConnected() = false;
                    newClient->isValidRequest() = true;
                    FD_SET(newClient->getSocket(), &this->write_backup);
                    logger.warning("Client " + Logger::to_string(newClient->getSocket()) + " retry-after");
                }
                else
                    FD_SET(newClient->getSocket(), &this->read_backup);
                fd_av.push_back(newClient->getSocket());

                clients.push_front(newClient);
                logger.connect("[SERVER]: New Client: " + logger.to_string(newClient->getSocket()) + ". Server: " + server_curr->getServerConfig().getHost() + ":" + logger.to_string(server_curr->getServerConfig().getPort()));
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
                    this->disconnectClient(client_curr);
                    it = clients.erase(it);
                    logger.warning(std::string("[SERVER]: Disconnecting from client socket: ") + logger.to_string(client_curr->getSocket()));
                    continue;
                }

                client_curr->getServerConfig() = this->getBestServer(client_curr);

                if (client_curr->isValidRequest())
                    FD_SET(client_curr->getSocket(), &this->write_backup);
                break;
            }
            else
                logger.notice(std::string("[SERVER]: Client Loop: FD_ISSET - read_pool: socket " + logger.to_string(client_curr->getSocket()) + " is not set"));

            if (FD_ISSET(client_curr->getSocket(), &this->write_pool))
            {
                if (client_curr->isValidRequest())
                {
                    std::string response;
                    std::map<std::string, std::string>::const_iterator it_h;

                    if (((((it_h = client_curr->getObjRequest().getHeaders().find("Transfer-Encoding")) != client_curr->getObjRequest().getHeaders().end())
                         && (it_h->second.compare(0, 7, "chunked") == 0)) && !(client_curr->isChunked())) || client_curr->getObjResponse().getBody().size() > 30000)
                    {
                        client_curr->isChunked() = true;
                    }



                    if (client_curr->isChunked() == true)
                    {
                        if (client_curr->isFirstThrough() == true)
                        {
                            client_curr->getObjResponse().sendResponse(client_curr);
                            response = client_curr->getObjResponse().stringifyHeaders();
                            client_curr->isFirstThrough() = false;
                        }
                        else
                        {

                        }

                    }

//                    std::cout << RED_TEXT << "------------ RESPONSE -----------" << COLOR_RESET << std::endl;
//                    std::cout << GREY_TEXT << response << COLOR_RESET << std::endl;
//                    std::cout << RED_TEXT << "-------------- END --------------" << COLOR_RESET << std::endl;

                    int send_ret = 0;
                    if ((send_ret = send(client_curr->getSocket(), response.c_str(), response.length(), 0)) != (int) response.length())
                    {
                        if (send_ret == -1)
                            return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), -1));
                        if (send_ret == 0)
                            client_curr->isConnected() = false;
                    }

                    if (!client_curr->isConnected())
                    {
                        FD_CLR(client_curr->getSocket(), &this->write_backup);
                        FD_CLR(client_curr->getSocket(), &this->write_pool);
                        this->disconnectClient(client_curr);
                        clients.erase(it);
                        logger.warning(std::string("[SERVER]: Disconnecting from client socket: ") + logger.to_string(client_curr->getSocket()));
                        break;
                    }
                    logger.success("[SERVER]: Client : " + logger.to_string(client_curr->getSocket()) + ". Response send: file: " + client_curr->getObjRequest().getPath() + ". code: " + client_curr->getObjResponse().getStatusCode() + ".");

                }
                if (client_curr->isChunked() == true)
                {
                        std::cout << "is chunked :D\n";
                }
                else
                {
                    FD_CLR(client_curr->getSocket(), &this->write_backup);
                    client_curr->isValidRequest() = false;
                }
                break;
            }
            else
                logger.notice(std::string("[SERVER]: Client Loop: FD_ISSET - write_pool: socket " + logger.to_string(client_curr->getSocket()) + " is not set"));
        }
    }
}

std::list<Server *> &ServerManager::getServerList()
{
    return (this->servers);
}
void ServerManager::disconnectClient(Client *client)
{
    fd_av.remove(client->getSocket());
    client->close_socket();
}

void ServerManager::set_global_config(Config &conf)
{
    this->configGeneral = conf;
}

ServerConfig ServerManager::getBestServer(Client *client)
{
    std::vector<ServerConfig> tmp_conf;

    for (std::vector<ServerConfig>::iterator it = this->configGeneral.getServers().begin(); it != this->configGeneral.getServers().end(); it++)
    {
        if ((*it).getPort() == client->getListener())
            tmp_conf.push_back(*it);
    }

    std::map<std::string, std::string>::const_iterator header = client->getObjRequest().getHeaders().find("Host");
    for (std::vector<ServerConfig>::iterator it = tmp_conf.begin(); it != tmp_conf.end(); it++)
    {
        std::string host;

        if (header != client->getObjRequest().getHeaders().end())
        {
            if ((*header).second.find(it->getServerName()) != std::string::npos)
                host = (*header).second.substr(0, (*it).getServerName().size());

            if (header != client->getObjRequest().getHeaders().end())
            {
                if ((*it).getServerName() == host)
                    return ((*it));
            }
        }
    }

    return (tmp_conf.front());
}

