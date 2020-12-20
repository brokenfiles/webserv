#include "Client.hpp"

Client::Client() : socket(-1), ip(), port(-1), request_send()
{
    memset(&this->client_addr, 0, sizeof(client_addr));
}

Client::Client(int fd, struct sockaddr_in address) : socket(fd), client_addr(address), request_send()
{
    ip = std::string(inet_ntoa(client_addr.sin_addr));
    port = ntohs(client_addr.sin_port);
}


Client::~Client()
{

}

Client::Client(const Client &copy)
{
    *this = copy;
}

Client &Client::operator=(const Client &copy)
{
    if (this != &copy)
    {

    }
    return (*this);
}

int Client::getSocket(void)
{
    return (this->socket);
}

std::string Client::getRequest(void)
{
    return (this->request_send);
}

std::string Client::getIP(void)
{
    return (ip);
}

int Client::getPort()
{
    return (port);
}

void Client::setRequest()
{

}