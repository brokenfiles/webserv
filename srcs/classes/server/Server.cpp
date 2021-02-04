#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

Server::Server(const Server &copy)
{

    *this = copy;
}

Server &Server::operator=(const Server &copy)
{
    if (this != &copy)
    {
//        this->serverConfig = copy.serverConfig;
//        this ->server_sock = copy.server_sock;
//        this->serv_socket_in = copy.serv_socket_in;
    }
    return (*this);
}

int &Server::getServerSocket(void)
{
    return (this->server_sock);
}

struct sockaddr_in &Server::getServerAddr()
{
    return (this->serv_socket_in);
}

void Server::setServerConfig(ServerConfig &conf)
{
    serverConfig = conf;
}

ServerConfig &Server::getServerConfig(void)
{
    return (this->serverConfig);
}