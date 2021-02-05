#include "Client.hpp"

Client::Client() : socket(-1), port(-1), ip(), _recvRequest(), _parsedRequest()
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

int Client::read_request(void)
{
    char buffer[BUFFER];
    int read = BUFFER - 1;
    std::string keeper("");

    while (read == (BUFFER - 1))
    {
        memset(buffer, 0, BUFFER);
        read = recv(this->getSocket(), buffer, BUFFER - 1, 0);
        if (read < 1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                return (-2);
            else if (read == 0)
                std::cout << "0 returned after recv" << std::endl;
            else
                return (logger.error("[SERVER]: Could'nt read request: " + std::string(strerror(errno)), NO_PRINT_CLASS,-1));
        }
        keeper += buffer;
    }
    this->setRequest(keeper);
    logger.info("[SERVER]: data received", NO_PRINT_CLASS);
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
    std::cout << RED_TEXT << "------------ REQUEST ------------" << COLOR_RESET << std::endl;
    std::cout << GREY_TEXT << getStringRequest() << COLOR_RESET << std::endl;
    std::cout << RED_TEXT << "-------------- END --------------" << COLOR_RESET << std::endl;
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




