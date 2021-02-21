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

int Server::create_socket()
{
    if ((this->getServerSocket() = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return (logger.error("[SERVER]: socket: " + std::string(strerror(errno)), -1));

    if (fcntl(this->getServerSocket(), F_SETFL, O_NONBLOCK) < 0)
        return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), -1));

    memset(&this->getServerAddr(), 0, sizeof(struct sockaddr_in));
    this->getServerAddr().sin_family = AF_INET;
    this->getServerAddr().sin_port = htons(serverConfig.getPort());
    this->getServerAddr().sin_addr.s_addr = inet_addr(serverConfig.getHost().c_str());

    int opt = 1;
    if ((setsockopt(this->getServerSocket(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int))) < 0)
        return (logger.error("[SERVER]: setsockopt: " + std::string(strerror(errno)), -1));

    if (bind(this->getServerSocket(), (const struct sockaddr *) &this->getServerAddr(), sizeof(this->getServerAddr())) < 0)
        return (logger.error("[SERVER]: bind: " + std::string(strerror(errno)), -1));

    if (listen(this->getServerSocket(), 20) < 0)
        return (logger.error("[SERVER]: listen" + std::string(strerror(errno)), -1));
    return (0);
}

int Server::accept_client(Client *client)
{
    int size = sizeof(client->getAddr());

    if ((client->getSocket() = accept(this->getServerSocket(), (struct sockaddr *) &client->getAddr(), (socklen_t *) &size)) < 0)
        return (logger.error("[SERVER]: accept: " + std::string(strerror(errno)), -1));

    client->getIP() = std::string(inet_ntoa(client->getAddr().sin_addr));
    client->getPort() = ntohs(client->getAddr().sin_port);

    if (fcntl(client->getSocket(), F_SETFL, fcntl(client->getSocket(), F_GETFL) | O_NONBLOCK) < 0) {
        return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), -1));
    }
    return (0);
}
