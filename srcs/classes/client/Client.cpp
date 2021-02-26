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

    if (!this->request.isHeaderParsed() && keeper.find("\r\n\r\n") != std::string::npos)
    {
        std::cout << keeper << std::endl;
        std::cout << "PARSING HEADER\n";
        this->parser.parseHeader(this->request, keeper);
    }

    if (this->request.isHeaderParsed() && !this->request.isBodyParsed())
    {
        std::cout << "-" << keeper << "-" << std::endl;
        std::cout << "PARSING BODY" << std::endl;
        this->parser.parseBody(this->request, keeper);
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

    //Si pas de CRLF, on continue de read sur le socket jusqu'à une fin de patern
    this->_recvRequest_backup = keeper;
    logger.warning("[SERVER]: Client: " + logger.to_string(this->getSocket()) + " Request non completed. Valid Request: " + logger.to_string(this->validRequest) + ". backup size: " + logger.to_string(this->_recvRequest_backup.size()) + ".");
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
Parser &Client::getObjParser()
{
    return (this->parser);
}
void Client::encode_chunk(Response &rep, std::string &response)
{
    logger.warning("PERFORMING CHUNKED RESPONSE");
    if (this->isFirstThrough())
    {
        rep.sendResponse(this);
        rep.setHeader("Transfer-Encoding", "chunked");
        rep.removeHeader("Content-Length");
        this->headerstring = rep.stringifyHeaders();
        this->bodystring = rep.getBody();

        response = this->headerstring;
        this->isFirstThrough() = false;
        if (this->bodystring.empty())
        {
            response += "0\r\n\r\n";
            this->isChunked() = false;
        }
    }
    else
    {
        std::string finalchunk;
        size_t size = 0;

        if (this->bodystring.size() >= 1000001)
            size = 1000001;
        else
            size = this->bodystring.size();

        std::stringstream convert;
        convert << std::hex << size;
        std::string size_hex = convert.str();

        finalchunk += (size_hex + "\r\n");
        finalchunk += (this->bodystring.substr(0, size) + "\r\n");

        if (this->bodystring.size() <= 1000001)
        {
            finalchunk += "0\r\n\r\n";
            this->isChunked() = false;
        }
        this->bodystring = this->bodystring.erase(0, size);
        if (this->bodystring.empty())
            this->isChunked() = false;
        logger.warning("[SERVER]: Sending single chunk with size of: " + Logger::to_string(size) + ", size left: " + Logger::to_string(
                this->bodystring.size()));

        response = finalchunk;
    }
}
void Client::clear_state()
{
    this->isValidRequest() = false;
    this->isFirstThrough() = true;
    this->bodystring.clear();
    this->headerstring.clear();
    this->getObjRequest().setBodyRaw("");
}
void Client::checkIfIsChunked()
{
    std::map<std::string, std::string>::const_iterator it_h;
    if ((!(this->isChunked()) && (((it_h = this->getObjRequest().getHeaders().find("Transfer-Encoding")) != this->getObjRequest().getHeaders().end())
                                         && (it_h->second.compare(0, 7, "chunked") == 0))))
    {
        this->isChunked() = true;
    }
}

void Client::printswagresponse(std::string &str)
{
    if (!logger.isSilent())
    {
        std::cout << RED_TEXT << "------------ RESPONSE -----------" << COLOR_RESET << std::endl;
        std::cout << GREY_TEXT << str << COLOR_RESET << std::endl;
        std::cout << RED_TEXT << "-------------- END --------------" << COLOR_RESET << std::endl;
    }
}
int Client::send_response(std::string &response)
{
    int send_ret;
    if ((send_ret = send(this->getSocket(), response.c_str(), response.length(), 0)) != (int) response.length())
    {
        if (send_ret == -1)
            return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), -1));
        if (send_ret == 0)
            this->isConnected() = false;
    }
    return (1);
}






