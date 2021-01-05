#include "Response.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

Response::Response()
{}

Response::Response(Response &response) : Query(response)
{}

Response::~Response()
{}

void Response::setStatus(const std::string &status) {
    _status = status;
}

const std::string &Response::getStatus() const {
    return _status;
}

//assemble les elements de la requete dans une string et la renvoi
std::string 	Response::stringify(void) const
{
    std::string		ret;
    std::map<std::string, std::string>::const_iterator	ite;

    ret.insert(ret.length(), "HTTP/1.1 ");
    ret.insert(ret.length(), getStatus());
    ret.push_back('\r');
    ret.push_back('\n');
    for(ite = getHeaders().begin(); ite != getHeaders().end(); ite++)
    {
        ret.insert(ret.length(), ite->first);
        ret.push_back(':');
        ret.push_back(' ');
        ret.insert(ret.length(), ite->second);
        ret.push_back('\n');
    }
    ret.push_back('\r');
    ret.push_back('\n');
    ret.insert(ret.length(), getBody());
    return (ret);
}

std::map<std::string, std::string>	Response::basicHeaders(void)
{
	std::map<std::string, std::string> map;

	map["Content-Type"] = "text/html";
	map["Server"] = "Webserv";
	map["Date"] = getCurrentTime();
	return (map);
}

void 	Response::fileExtension(std::map<std::string, std::string> *map, Request request)
{
	if (request.getPath().compare(request.getPath().length() - 4, 4, ".css") == 0)
		(*map)["Content-Type"] = "text/css";
	else if (request.getPath().compare(request.getPath().length() - 3, 3, ".js") == 0)
		(*map)["Content-Type"] = "application/javascript";
}

void	Response::addBody(std::string path)
{
	std::string newStr;
	std::ifstream file(path.c_str(), std::ifstream::in);
	std::getline(file, newStr, '\0');
	setBody(newStr);
	file.close();
}

void 	Response::getHandler(Request request, int head)
{
	std::map<std::string, std::string> map;
	std::string path = "home";

	map = basicHeaders();
	if (request.getPath() == "/")
		path.insert(path.length(), "/index.html");
	else
	{
		path.insert(path.length(), request.getPath());

		std::ifstream ifs(path.c_str(), std::ifstream::in);

		if (ifs.good())
		{
			if (ifs.is_open())
				fileExtension(&map, request);
			else
				std::cout << "can't open file" << std::endl;
		}
		else
		{
			setStatus("404 Not Found");
			path = "server/NotFound.html";
		}
		ifs.close();
	}
	if (head == 0)
		addBody(path);
	setHeaders(map);
}

void 	Response::putHandler(Request request)
{
	std::map<std::string, std::string> map;
	setStatus("201 Created");
	map = basicHeaders();
	int fd;
	std::string	path = request.getPath();

	path.erase(0, 1);
	if ((fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		return ;
	if (!write(fd, request.getBody().c_str(), request.getBody().length()))
	{
		std::cout << "test" << std::endl;
		return ;
	}
	close(fd);
	addBody("server/put.html");
	setHeaders(map);
}

void 	Response::deleteHandler(Request request)
{
	std::map<std::string, std::string> map;
	map = basicHeaders();
	std::string	path = request.getPath();

	path.erase(0, 1);
	if (remove(path.c_str()) == -1)
		return ;
	addBody("server/delete.html");
	setHeaders(map);
}

void	Response::prepareResponse(std::string req)
{
    Parser parser;
	Request	request;

	request = parser.parse(req);

	setStatus("200 OK");

	if (request.getMethod() == "GET")
    	getHandler(request, 0);
	else if (request.getMethod() == "HEAD")
		getHandler(request, 1);
	else if (request.getMethod() == "PUT")
		putHandler(request);
	else if (request.getMethod() == "DELETE")
		deleteHandler(request);
}

//fonction qui donne la date au format HTTP dans une string
std::string Response::getCurrentTime(void)
{
    struct timeval tv;
    std::string ret;
    char str[64];
    time_t time;
    struct tm *current = NULL;

    gettimeofday(&tv, NULL);
    time = tv.tv_sec;
    current = localtime(&time);	//localtime est interdite faudra la recoder a l'occas
    strftime(str, sizeof str, "%a, %d %b %G %T GMT", current);
    ret.assign(str);
    return (ret);
}

std::ofstream&	operator<<(std::ofstream &o, const Response &res)
{
	o << res.stringify();
	return (o);
}