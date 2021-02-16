#include "Parser.hpp"

Parser::Parser(void)
{

}

Parser::~Parser(void)
{

}


void Parser::parseHeader(Request &req, std::string& keeper)
{
    std::string frontLine = keeper.substr(0, keeper.find('\n'));
    try
    {
        this->fillMethod(req, frontLine);
        this->fillPath(req, frontLine);

    }
    catch (const std::exception &e)
    {
        logger.error("[SERVER]: " + logger.to_string(e.what()), NO_PRINT_CLASS, -1);
    }

    /* remove front line */
    keeper.erase(0, frontLine.length() + 1);

    /* fill header */
    this->fillHeader(req, keeper);
    this->fillQueryString(req);

}


int Parser::fillChunk(std::string &keeper)
{
    if (keeper.find("\r\n\r\n") != std::string::npos)
        return (1);
    return (0);

//    std::string chunk = keeper.substr(0, keeper.find('\r'));
//    std::stringstream convert;
//    int size_chunk;

//    keeper.erase(0, chunk.size() + 2);
//    convert << chunk;
//    convert >> std::hex >> size_chunk;
//
//    std::cout << "size chunked: " << size_chunk << std::endl;
}

int Parser::fillContentSize(std::string &keeper, std::string strsize)
{
    std::stringstream convert;
    ulong size;

    convert << strsize;
    convert >> size;

    if (keeper.size() == size)
        return (1);
    return (0);
}

Query	Parser::parseResponse(std::string strResponse)
{
	Request req;

	this->fillHeader(req, strResponse);

	return (req);
}

void Parser::fillMethod(Request &req, std::string &frontLine)
{
    std::string methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
    std::string reqMethod = frontLine.substr(0, frontLine.find(' '));


    //Check la reqMethod
    for (size_t i = 0; i < 9; i++)
    {
        if (methods[i] == reqMethod)
        {
            req.setMethod(reqMethod);
            break;
        }
    }

    if (req.getMethod().empty())
        throw BadRequestMethod();
}

void Parser::fillPath(Request &req, std::string &frontLine)
{
    //get full path (ex: /index.htlm?oui=ahahah)
    size_t start = frontLine.find("/");
    size_t path_length = frontLine.find(" ", start);

    std::string fullPath = frontLine.substr(start, path_length - start);

    req.setPath(fullPath);
}

void Parser::fillHeader(Request& req, std::string& keeper)
{
    std::map<std::string, std::string> map;
    size_t x;

    //récupère ligne par ligne, stock "key:value" dans map, et erase la ligne
    while ((x = keeper.find('\n')) != std::string::npos)
    {
        if (x == 1)
        {
            keeper.erase(0, 2);
            break;
        }
        std::string line = keeper.substr(0, x);
        if (line.find(':') != std::string::npos)
        {
            if (line.substr(0, line.find(':')) == "Set-Cookie")
                req.addCookie(line.substr(line.find(':') + 2, x - 2 - line.find(':')));
            else
                map[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2, x - 2 - line.find(':'));
            keeper.erase(0, x + 1);
        }
        else
            throw BadHeader();
    }
    req.setHeaders(map);
    req.isHeaderParsed() = true;
}

void Parser::fillBody(Request& req, std::string& strRequest)
{
    req.setBody(strRequest);
}

void Parser::fillQueryString(Request &req)
{
    size_t breakPoint;

    if ((breakPoint = req.getPath().find('?', 0)) != std::string::npos)
    {
        //On récupère dans setQueryString ce qu'il y a après le '?' (ex: GET /index.html?plop=plup), puis setPath sans le QueryString
        req.setQueryString(req.getPath().substr(breakPoint + 1, req.getPath().length() - breakPoint - 1));
        req.setPath(req.getPath().substr(0, breakPoint));
    }
    else
    {
        //Sinon pas de QueryString
        req.setQueryString("");
    }

}
