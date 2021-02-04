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
        Server* current = (*it_serv);
        
        current->setServerConfig((*it_conf));

        if ((current->getServerSocket() = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            return (logger.error("[SERVER]: socket: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        if (fcntl(current->getServerSocket(), F_SETFL, O_NONBLOCK) < 0)
            return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        memset(&current->getServerAddr(), 0, sizeof(struct sockaddr_in));
        current->getServerAddr().sin_family = AF_INET;
        current->getServerAddr().sin_port = htons((*it_conf).getPort());
        current->getServerAddr().sin_addr.s_addr = inet_addr((*it_conf).getHost().c_str());

        if (bind(current->getServerSocket(), (const struct sockaddr *) &current->getServerAddr(), sizeof(current->getServerAddr())) < 0)
            return (logger.error("[SERVER]: bind: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        int opt = 1;
        if ((setsockopt(current->getServerSocket(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))) < 0)
            return (logger.error("[SERVER]: setsockopt: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        if (listen(current->getServerSocket(), 20) < 0)
            return (logger.error("[SERVER]: listen" + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        logger.info("[SERVER]: " + current->getServerConfig().getHost() + ":" + logger.to_string(current->getServerConfig().getPort()) + " is now listening", NO_PRINT_CLASS);
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

int ServerManager::read_request(Client* client)
{
    char buffer[BUFFER];
    int read = BUFFER - 1;
    std::string keeper("");

    //savoir si un fd est nonblockant
//    if(fcntl(client->getSocket(), F_GETFL) & O_NONBLOCK) {
//        std::cout << client->getSocket() << " is blocking" << std::endl;
//    }

    while (read == (BUFFER - 1))
    {
        memset(buffer, 0, BUFFER);
        read = recv(client->getSocket(), buffer, BUFFER - 1, 0);
        if (read < 1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
//                std::cout << "EWOULDBLOCK || EAGAIN returned after recv" << std::endl;
                return (-100);
            }
            else if (read == 0)
                std::cout << "0 returned after recv" << std::endl;
            else
                return (logger.error("[SERVER]: Could'nt read request: " + std::string(strerror(errno)), NO_PRINT_CLASS,-1));
        }
        keeper += buffer;
    }
    client->setRequest(keeper);
    logger.info("[SERVER]: data received", NO_PRINT_CLASS);
    return (0);
}


int ServerManager::send_request(int fd, std::string toSend)
{
    if (send(fd, toSend.c_str(), toSend.length(), 0) != (int) toSend.length())
        return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
    return (0);
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
                int size = sizeof(newClient->getAddr());

                if ((newClient->getSocket() = accept(server_curr->getServerSocket(), (struct sockaddr *) &newClient->getAddr(), (socklen_t *) &size)) < 0)
                    return (logger.error("[SERVER]: accept: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

                if (newClient->getSocket() > higher_fd)
                    higher_fd = newClient->getSocket();
                FD_SET(newClient->getSocket(), &fd_pool);

                if(fcntl(newClient->getSocket(), F_SETFL, fcntl(newClient->getSocket(), F_GETFL) | O_NONBLOCK) < 0) {
                    return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
                }
                logger.notice("[SERVER]: New Client connexion: " + logger.to_string(newClient->getSocket()) , NO_PRINT_CLASS);
                clients.push_front(newClient);
            }

            for (std::list<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
            {
                Client *client_curr = (*it);

                if (FD_ISSET(client_curr->getSocket(), &fd_pool))
                {
                    int code;
                    if ((code = this->read_request(client_curr)) < 0)
                    {
                        if (code == -100)
                            break;
                        return (-1);
                    }
                    std::cout << "request:\n";
                    std::cout << client_curr->getRequest() << std::endl;

//                    (void)env;
                    Response rep;
                    rep.prepareResponse(client_curr->getRequest(), env);
                    rep.stringify();

                    if (this->send_request(client_curr->getSocket(), rep.stringify()) == -1)
                        return (-1);
//                    if (this->send_request(client_curr->getSocket(), "omgwhatawoowww") == -1)
//                        return (-1);

                    close(client_curr->getSocket());
                    logger.notice(std::string("[SERVER]: Disconnecting from client sock: ") + logger.to_string(client_curr->getSocket()), NO_PRINT_CLASS);
                    it = clients.erase(it);
                }
            }
        }
    }
}

std::list<Server *> &ServerManager::getServerList()
{
    return (this->servers);
}