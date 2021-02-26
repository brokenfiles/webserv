#include <algorithm>
#include "Parser.hpp"

Parser::Parser(void)
{

}

Parser::~Parser(void)
{

}


void Parser::parseHeader(Request &req, std::string& keeper)
{
    //std::cout << "-------------- REQUEST BEFORE PARSING -----------------" << std::endl;
    //std::cout << keeper << std::endl;
    //std::cout << "-------------------------------------------------------\n";
    std::string frontLine = keeper.substr(0, keeper.find('\n'));
    try
    {
        this->fillMethod(req, frontLine);
        this->fillPath(req, frontLine);

        /* remove front line */
        keeper.erase(0, frontLine.length() + 1);

        /* fill header */
		this->fillHeader(req, keeper);
        this->fillQueryString(req);

    }
    catch (const std::exception &e)
    {
        keeper.clear();
        logger.error("[SERVER]: " + logger.to_string(e.what()), -1);
    }
    //            std::cout << "-------------- REQUEST AFTER PARSING ------------------" << std::endl;
    //            std::cout << ">" << keeper << "< size:" << keeper.size() << std::endl;
    //            std::cout << "-------------------------------------------------------\n";
}


int Parser::fillChunk(std::string &keeper, Request& request)
{
    size_t x;

    while (1)
    {
        if ((x = keeper.find("\r\n")) != std::string::npos)
        {
            std::string line = keeper.substr(0, x);
            if (!line.empty())
            {
                int size_chunk = 0;
                std::stringstream convert;
                convert << std::hex << line;
                convert >> size_chunk;
                std::cout << "keeper size : " << keeper.size() << std::endl;
                std::cout << "line size : " << line.size() << std::endl;
                std::cout << "(keeper.size() - (line.size() + 2) : " << keeper.size() - line.size() + 2 << std::endl;
                std::cout << "condition : " << ((keeper.size() - (line.size() + 2)) >= (size_t) size_chunk + 2) << std::endl;
                std::cout << "size_chunk : " << size_chunk << std::endl;
                std::cout << "size_chunk + 2 : " << size_chunk + 2 << std::endl;
//                std::cout << "le chunk fait : " << size_chunk << std::endl;

                if (size_chunk > 0 && ((keeper.size() - (line.size() + 2)) >= (size_t) size_chunk + 2))
                {
                    std::cout << "PARSER: fillChunk perform chunk : " << size_chunk << std::endl;
                    request.appendBody(keeper.substr(x + 2, size_chunk));
                    keeper.erase(0, size_chunk + x + 4);
                    continue;
                }

                if (size_chunk == 0 && keeper.find("\r\n\r\n") != std::string::npos)
                    return (1);
            }

            if (keeper.find("\r\n\r\n") != std::string::npos)
                continue;
            else
                return (0);

        }
        else
            return (0);
    }
    return (0);
}

int Parser::fillContentSize(std::string &keeper, std::string strsize)
{
    if (keeper.find("\r\n\r\n") != std::string::npos)
    {
        std::stringstream convert;
        unsigned long size;

        convert << strsize;
        convert >> size;

        if (keeper.size() == size)
            return (1);
    }
    return (0);
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

//    if (req.getMethod().empty())
//        throw BadRequestMethod();
}

void Parser::fillPath(Request &req, std::string &frontLine)
{
    //get connected path (ex: /index.htlm?oui=ahahah)
    size_t start = frontLine.find("/");
    size_t path_length = frontLine.find(" ", start);

    std::string fullPath = frontLine.substr(start, path_length - start);

    req.setPath(fullPath);
}

Request	Parser::parseResponse(std::string strResponse)
{
	Request req;

	this->fillHeader(req, strResponse);

	return (req);
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
//        if (line.at(line.size() - 1) == '\r')
//        	line.erase(line.size() - 1);
		if (line.find(':') != std::string::npos)
        {
			if (line.substr(0, line.find(':')) == "Set-Cookie") {
				req.addCookie(line.substr(line.find(':') + 2, x - 3 - line.find(':')));
			}
            else
                map[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2, x - 3 - line.find(':'));
            keeper.erase(0, x + 1);
        }
        else {
			break;
		}
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

void Parser::parseBody(Request &req, std::string &keeper)
{
    std::map<std::string, std::string>::const_iterator it;
    if ((((it = req.getHeaders().find("Transfer-Encoding")) != req.getHeaders().end()) && (it->second.compare(0, 7, "chunked") == 0)))
    {
    	std::cout << "Calling fillchunk" << std::endl;
    	int ret;
        if ((ret = this->fillChunk(keeper, req)))
            req.setBody(req.getBody());
//		(void)ret;
        std::cout << "fillchunk returned " << ret << std::endl;
    }
    else if ((it = req.getHeaders().find("Content-Length")) != req.getHeaders().end())
    {
        if (this->fillContentSize(keeper, (*it).second))
            req.setBody(keeper);
    }
    else
        req.setBody(keeper);
}
