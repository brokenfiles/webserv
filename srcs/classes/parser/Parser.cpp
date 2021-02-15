#include "Parser.hpp"

Parser::Parser(void)
{

}

Parser::~Parser(void)
{

}

Request	Parser::parse(std::string strRequest)
{
	Request req;

	//get frontLine of strRequest (Method + Path)
    std::string frontLine = strRequest.substr(0, strRequest.find('\n'));

    try
    {
        this->fillMethod(req, frontLine);
    }
    catch (const std::exception &e)
    {
        logger.error("[SERVER]: " + logger.to_string(e.what()), NO_PRINT_CLASS, -1);
    }
    this->fillPath(req, frontLine);


    //remove frontLine from strRequest (useless now because already parsed)
    strRequest.erase(0, frontLine.length() + 1);

    this->fillHeader(req, strRequest);
    this->fillBody(req, strRequest);

    //Parsing to get QueryString
    this->fillQueryString(req);

	return (req);
}

Query	Parser::parseResponse(std::string strResponse)
{
	Query req;

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

    //TODO (si request vide (ex: Telnet))
    if (req.getMethod() == "")
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

void Parser::fillHeader(Query& req, std::string& strRequest)
{
    std::map<std::string, std::string> map;

    size_t x;

    //récupère ligne par ligne, stock "key:value" dans map, et erase la ligne
    while ((x = strRequest.find('\n')) != std::string::npos)
    {
        if (x == 1)
        {
            strRequest.erase(0, 2);
            break;
        }
        std::string line = strRequest.substr(0, x);
        if (line.substr(0, line.find(':')) == "Set-Cookie") {
        	req.addCookie(line.substr(line.find(':') + 2, x - 2 - line.find(':')));
        } else {
			map[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2, x - 2 - line.find(':'));
		}
        strRequest.erase(0, x + 1);
    }
    req.setHeaders(map);
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
