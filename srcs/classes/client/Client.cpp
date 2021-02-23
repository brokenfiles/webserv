#include "Client.hpp"

Client::Client() : request(), socket(-1), port(-1), ip(), _recvRequest(), _recvRequest_backup()
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

    if (!this->_recvRequest_backup.empty())
        keeper = this->_recvRequest_backup;

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

    //Si on trouve un CRLF "\r\n\r\n" > Parsing HEADER || BODY;
    if (keeper.find("\r\n\r\n") != std::string::npos)
    {
        //Si Header PAS encore parsé, on parse =)
        if (!this->request.isHeaderParsed())
        {
        //    std::cout << "-------------- REQUEST BEFORE PARSING -----------------" << std::endl;
        //    std::cout << keeper << std::endl;
        //    std::cout << "-------------------------------------------------------\n";
            this->parser.parseHeader(this->request, keeper);
//            std::cout << "-------------- REQUEST AFTER PARSING ------------------" << std::endl;
//            std::cout << ">" << keeper << "< size:" << keeper.size() << std::endl;
//            std::cout << "-------------------------------------------------------\n";
        }

        std::map<std::string, std::string>::const_iterator it;

        //Ensuite Parsing du BODY si header déjà parsé
        if (this->request.isHeaderParsed() && !this->request.isBodyParsed())
        {
            //SI "Transfer-Encoding: chunked", il y a un body et il faut le récupérer entierement =)
            if ((((it = this->request.getHeaders().find("Transfer-Encoding")) != this->request.getHeaders().end()) && (it->second.compare(0, 7, "chunked") == 0)))
            {
                //Vrai si CRLF (pour l'instant, Récupéré chunk par chunk à l'avenir)
                if (this->parser.fillChunk(keeper))
                {
                    this->request.setBody(keeper);
                }
            }
            //SINON SI y'a Content-Length, = BODY, il faut le récupérer entierement également =)
            else if ((it = this->request.getHeaders().find("Content-Length")) != this->request.getHeaders().end())
            {
                //si size du body == content-length, vrai
                if (this->parser.fillContentSize(keeper, (*it).second))
                {
                    this->request.setBody(keeper);
                }

            }
            //Sinon pas de body, ça part là dessus ^^
            else
            {
                this->request.setBody(keeper);
            }
        }

        //Si HEADER et BODY récupéré/parsé, c'est une valid request, on continue =)
        if (this->request.isHeaderParsed() && this->request.isBodyParsed())
        {
            logger.success("[SERVER]: Client : " + logger.to_string(this->getSocket()) + ". Data received. Valid request: " + logger.to_string(this->validRequest) + ". size: " + logger.to_string(this->getStringRequest().size()) + ".", NO_PRINT_CLASS);
            this->request.isBodyParsed() = false;
            this->request.isHeaderParsed() = false;
            this->isValidRequest() = true;
            this->_recvRequest_backup.clear();
            return (0);
        }
    }
//    else if (!this->request.isHeaderParsed())
//        return (logger.error("[SERVER]: Not Header Found.", NO_PRINT_CLASS, -1));

    //Si pas de CRLF, on continue de read sur le socket jusqu'à une fin de patern
    this->_recvRequest_backup = keeper;
    logger.warning("[SERVER]: Client: Request non completed. Valid Request: " + logger.to_string(this->validRequest) + ". backup size: " + logger.to_string(this->_recvRequest_backup.size()) + ".", NO_PRINT_CLASS);
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




