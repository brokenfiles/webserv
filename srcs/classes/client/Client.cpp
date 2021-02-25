#include "Client.hpp"

Client::Client() : request(), socket(-1), port(-1), ip(), _recvRequest_backup()
{
    memset(&this->client_addr, 0, sizeof(client_addr));
    this->validRequest = false;
    this->connected = true;
    this->chunk_rep = false;
    this->firstThrough = true;
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
    bool check(false);
    int read;

    if (!this->_recvRequest_backup.empty())
        keeper = this->_recvRequest_backup;

    memset((void*)buffer, 0, BUFFER);

    while ((read = recv(this->getSocket(), buffer, BUFFER - 1, 0)) > 0)
    {
        buffer[read] = '\0';
        keeper += buffer;
        check = true;
    }

    if (!(check) || read == 0)
    {
        if (read == 0)
            return (logger.warning(std::string("[SERVER]: recv: 0")), -1);
        else
            return (logger.warning(std::string("[SERVER]: recv: -1: " + std::string(strerror(errno)))), -1);
    }

    if (keeper.find("\r\n\r\n") != std::string::npos)
    {
//        std::cout << RED_TEXT << "------------ REQUEST BEFORE PARSING -----------" << COLOR_RESET << std::endl;
//        std::cout << GREY_TEXT << keeper << COLOR_RESET << std::endl;
//        std::cout << RED_TEXT << "-------------- END --------------" << COLOR_RESET << std::endl;
        if (!this->request.isHeaderParsed())
            this->parser.parseHeader(this->request, keeper);

        std::map<std::string, std::string>::const_iterator it;

        if (this->request.isHeaderParsed() && !this->request.isBodyParsed())
        {
            if ((((it = this->request.getHeaders().find("Transfer-Encoding")) != this->request.getHeaders().end()) && (it->second.compare(0, 7, "chunked") == 0)))
            {
                if (this->parser.fillChunk(keeper))
                    this->request.setBody(keeper);
            }
            else if ((it = this->request.getHeaders().find("Content-Length")) != this->request.getHeaders().end())
            {
                if (this->parser.fillContentSize(keeper, (*it).second))
                    this->request.setBody(keeper);
            }
            else
                this->request.setBody(keeper);
        }

        if (this->request.isHeaderParsed() && this->request.isBodyParsed())
        {
            this->request.isBodyParsed() = false;
            this->request.isHeaderParsed() = false;
            this->isValidRequest() = true;
            logger.success("[SERVER]: Client: " + logger.to_string(this->getSocket()) + " Request completed. Valid Request: " + logger.to_string(this->validRequest) + ". Total size: " + logger.to_string(this->request.getBody().size()) + ".");
            this->_recvRequest_backup.clear();
            return (0);
        }
    }

    //Si pas de CRLF, on continue de read sur le socket jusqu'à une fin de patern
    this->_recvRequest_backup = keeper;
    logger.warning("[SERVER]: Client: Request non completed. Valid Request: " + logger.to_string(this->validRequest) + ". backup size: " + logger.to_string(this->_recvRequest_backup.size()) + ".");
    return (0);
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

bool &Client::isValidRequest()
{
    return (this->validRequest);
}
bool &Client::isConnected()
{
    return (this->connected);
}

int &Client::getListener()
{
    return (this->listen);
}
bool &Client::isChunked()
{
    return (this->chunk_rep);
}
bool &Client::isFirstThrough()
{
    return (this->firstThrough);
}






