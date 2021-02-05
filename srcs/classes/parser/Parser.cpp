#include "Parser.hpp"
#include <vector>
#include <list>

Parser::Parser(void)
{

}

Parser::~Parser(void)
{

}

void 	Parser::fillMethodAndPath(Request &req, std::string strRequest)
{
    std::string methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

    //stock la première ligne de la request;
    std::string front = strRequest.substr(0, strRequest.find('\n'));

    size_t breakPoint = front.find(' ');
    std::string method = front.substr(0, breakPoint);

    //recupère la request method
    for (size_t i = 0; i < methods->length(); i++)
    {
        if (methods[i] == method)
        {
            req.setMethod(method);
            break;
        }
    }

    //recupère le path
    breakPoint = front.find("/");
    size_t path_length = front.find(" ", breakPoint);
    req.setPath(front.substr(breakPoint, path_length - breakPoint));

    if (req.getPath() == "" || req.getMethod() == "")
        throw BadRequestMethod();
}

void Parser::fillHeaderAndBody(Request &req, std::string strRequest)
{
    std::map<std::string, std::string> map;
    bool first_line = true;
    size_t x;

    while ((x = strRequest.find('\n')) != std::string::npos)
    {
        if (x == 1)
        {
            strRequest.erase(0, 2);
            break;
        }
        if (!first_line)
        {
            std::string line = strRequest.substr(0, x);
            map[line.substr(0, line.find(':'))] = line.substr(line.find(':') + 2, x - 2 - line.find(':'));
        }
        else
            first_line = !first_line;
        strRequest.erase(0, x + 1);
    }
    req.setHeaders(map);
    req.setBody(strRequest);
}

Request	Parser::parse(std::string strRequest)
{
	Request request;
    size_t  x;

    fillMethodAndPath(request, strRequest);
    fillHeaderAndBody(request, strRequest);


//    if (request.getMethod() == "POST")// && request.getHeaders().find("Content-Type")->second == "application/x-www-form-urlencoded")
//        request.setQueryString(query);
//    else
//        request.setQueryString("");

	if (request.getMethod() == "GET")
	{
		if ((x = request.getPath().find('?', 0)) != std::string::npos)
            request.setQueryString(request.getPath().substr(x + 1, request.getPath().length() - x - 1));
		request.setPath(request.getPath().substr(0, x));
	}
	return (request);
}