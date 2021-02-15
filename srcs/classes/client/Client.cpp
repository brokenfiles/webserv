#include "Client.hpp"

Client::Client() : socket(-1), port(-1), ip(), _recvRequest()
{
    memset(&this->client_addr, 0, sizeof(client_addr));
    this->connected = true;
    this->validRequest = false;
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

int Client::read_request(void)
{
    char buffer[BUFFER];
    std::string keeper("");
    bool recvCheck(false);
    int read;

//    c->resetTimeOut();

    memset((void*)buffer, 0, BUFFER);

    while ((read = recv(this->getSocket(), buffer, BUFFER, 0)) > 0)
    {
        buffer[read] = '\0';
        keeper += buffer;
        recvCheck = true;
    }


    if (!(recvCheck) || read == 0)
    {
        this->connected = false;
        this->validRequest = false;
        if (read == 0)
            return (logger.warning(std::string("[SERVER]: recv: 0"), NO_PRINT_CLASS), -1);
        else
            return (logger.warning(std::string("[SERVER]: recv: -1: " + std::string(strerror(errno))), NO_PRINT_CLASS), -1);
    }

    std::string methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
    for (size_t i = 0; i < 9; i++)
    {
        size_t pos = keeper.find(methods[i].c_str(), 0, methods[i].size());
        if (pos != std::string::npos)
        {
            logger.notice("[SERVER]: Method " + methods[i] + " found, perform parsing.", NO_PRINT_CLASS);
            this->validRequest = true;
        }
    }

    this->setRequest(keeper);


//    if (!this->isValidRequest())
//        return (-1);

    logger.success("[SERVER]: Client : " + logger.to_string(this->getSocket()) + ". Data received. Valid request: " + logger.to_string(this->validRequest) + ". size: " + logger.to_string(this->getStringRequest().size()) + ".", NO_PRINT_CLASS);
    this->printRequest();

    return (0);
}

int Client::send_response(const std::string &req)
{
    if (send(this->socket, req.c_str(), req.length(), 0) != (int) req.length())
        return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
    return (0);
}

void Client::printRequest(void)
{
//    if (!logger.isSilent())
//    {
        std::cout << RED_TEXT << "------------ REQUEST ------------" << COLOR_RESET << std::endl;
        std::cout << GREY_TEXT << getStringRequest() << COLOR_RESET << std::endl;
        std::cout << RED_TEXT << "-------------- END --------------" << COLOR_RESET << std::endl;
//    }
}

void Client::close_socket()
{
    close(this->socket);
}

struct sockaddr_in &Client::getAddr()
{
    return (client_addr);
}
int &Client::getSocket()
{
    return (socket);
}

void Client::setRequest(std::string& request)
{
    _recvRequest = request;
}

std::string& Client::getStringRequest(void)
{
    return (this->_recvRequest);
}

std::string& Client::getIP(void)
{
    return (ip);
}

int& Client::getPort()
{
    return (port);
}

Request &Client::getObjRequest()
{
    return (request);
}

ServerConfig &Client::getServerConfig()
{
    return (serverConfig);
}
bool &Client::isAvailable()
{
    return (this->connected);
}
bool &Client::isValidRequest()
{
    return (this->validRequest);
}




