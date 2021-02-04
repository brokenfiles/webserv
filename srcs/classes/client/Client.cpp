#include "Client.hpp"

Client::Client() : socket(-1), port(-1), ip(), request_send()
{
    memset(&this->client_addr, 0, sizeof(client_addr));
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

void Client::setRequest(std::string& request)
{
    request_send = request;
}

struct sockaddr_in &Client::getAddr()
{
    return (client_addr);
}
int &Client::getSocket()
{
    return (socket);
}


