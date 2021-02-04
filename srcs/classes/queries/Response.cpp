#include "Response.hpp"
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
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

bool	Response::isCGI(std::string path)
{
	int		nb = path.rfind('.', path.length());

	path = path.substr(nb + 1, path.length() - nb);

	if (path == "php" || path == "rb" || path == "bla")
		return (true);
	return (false);
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
    (void)request;
	if (getIndex().compare(getIndex().length() - 4, 4, ".css") == 0)
		(*map)["Content-Type"] = "text/css";
	else if (getIndex().compare(getIndex().length() - 3, 3, ".js") == 0)
		(*map)["Content-Type"] = "application/javascript";
	else if (getIndex().compare(getIndex().length() - 4, 4, ".bla") == 0)
		(*map)["Content-Type"] = "text/bla";
	else if (getIndex().compare(getIndex().length() - 4, 4, ".svg") == 0)
		(*map)["Content-Type"] = "images/svg";
	else if (getIndex().compare(getIndex().length() - 4, 4, ".gif") == 0)
		(*map)["Content-Type"] = "images.gif";
	else if (getIndex().compare(getIndex().length() - 4, 4, ".png") == 0)
		(*map)["Content-Type"] = "images/png";
}

std::map<std::string, std::string> Response::find_location(ServerConfig server)
{
    std::list<LocationConfig> location_list = server.getLocations();
    std::map<std::string, std::string>      location;

    for (std::list<LocationConfig>::iterator ite = location_list.begin(); ite != location_list.end(); ite++)
    {
        if ((*ite).configuration["path"] == getPath())
            return ((*ite).configuration);
	}
	for (std::list<LocationConfig>::iterator it = location_list.begin(); it != location_list.end(); it++)
	{
		if ((*it).configuration["path"] == "/")
			return ((*it).configuration);
	}
    return (location);
}

std::string			Response::execute_cgi(std::string str, std::string path, Request request, char **envp)
{
	Cgi			object;

	object.parse(str, path, request, envp);
	object.execute(request);
	return (object.getOutputBody());
}

void	Response::addBody(Request request, char **envp)
{
	std::string newStr;

	std::ifstream file(getCompletePath().c_str(), std::ifstream::in);
	std::getline(file, newStr, '\0');
	if (isCGI(getCompletePath()) && request.getMethod() == "POST")
		setBody(execute_cgi(newStr, getCompletePath(), request, envp));
	else
		setBody(newStr);
	file.close();
}

void                                Response::setErrorStatus(int status_code)
{
    if (status_code == 403)
    {
        setStatus("403 Forbidden");
        setCompletePath(getLocation().at("root"));
        this->_complete_path.insert(getCompletePath().length(), "/server/Forbidden.html");
    }
    else if (status_code == 404)
    {
        setStatus("404 Not Found");
        setCompletePath(getLocation().at("root"));
        this->_complete_path.insert(getCompletePath().length(), "/server/NotFound.html");
    }
    else if (status_code == 500)
    {
        setStatus("500 Internal Server error");
        setCompletePath(getLocation().at("root"));
        this->_complete_path.insert(getCompletePath().length(), "/server/InternalServerError.html");
    }
}

void    Response::fileExist(std::string file, std::map<std::string, std::string> map, Request request)
{
    std::ifstream ifs(file.c_str(), std::ifstream::in);
    if (ifs.good())
    {
        if (ifs.is_open())
            fileExtension(&map, request);
        else
            setErrorStatus(500);
    }
    else
        setErrorStatus(404);
    ifs.close();
}

void 	Response::getHandler(Request request, int head, char **envp, ServerConfig server)
{
	std::map<std::string, std::string>	    map;

	(void)server;
    map = basicHeaders();
    std::cout << getCompletePath() << std::endl;
    if (getLocation().find("extension") != getLocation().end())
        if (getIndex().compare(getIndex().length() - getLocation().at("extension").length(), getLocation().at("extension").length(), getLocation().at("extension")) != 0)
            setErrorStatus(403);
    if (getLocation().at("methods").find(request.getMethod(), 0) == std::string::npos)
        setErrorStatus(403);
    fileExist(getCompletePath().c_str(), map, request);
	addBody(request, envp);
	if((int)getBody().length() > server.getMaxBodySize())
        setBody(getBody().substr(0, server.getMaxBodySize()));
	map["Content-Length"] = Logger::to_string(getBody().length());
	if (head == 1)
		setBody("");
	setHeaders(map);
}

void 	Response::putHandler(Request request, char **envp, ServerConfig server)
{
	(void)envp;
    (void)server;
	std::map<std::string, std::string> map;
	setStatus("201 Created");
	map = basicHeaders();
	int fd;
	std::string	error_path = HOME;
	std::string	path = HOME;
	path.insert(path.length(), request.getPath());

	if ((fd = open(path.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		return ;
	if ((write(fd, request.getBody().c_str(), request.getBody().length())) == -1)
		return ;
	close(fd);
	error_path.insert(error_path.length(), "/server/put.html");
    setCompletePath(error_path);
    addBody(request, envp);
	map["Content-Length"] = Logger::to_string(getBody().length());
	setHeaders(map);
}

void 	Response::deleteHandler(Request request, char **envp, ServerConfig server)
{
    (void)server;
    std::map<std::string, std::string> map;
	map = basicHeaders();
	std::string error_path = HOME;
	std::string	path = HOME;
	path.insert(path.length(), request.getPath());

	if (remove(path.c_str()) == -1)
		return ;
	error_path.insert(error_path.length(), "/server/deleted.html");
	setCompletePath(error_path);
	addBody(request, envp);
	map["Content-Length"] = Logger::to_string(getBody().length());
	setHeaders(map);
}

void        Response::init_location(ServerConfig server, Request request)
{
    std::map<std::string, std::string>      configMap = server.getConfiguration();
    setPath(request.getPath());

    if (getPath().find('.', 0) != std::string::npos)
    {
        setIndex(getPath().substr(getPath().rfind('/', getPath().length()), getPath().length() - getPath().rfind('/', getPath().length())));
        setPath(getPath().substr(0, getPath().rfind('/', getPath().length())));
        setLocation(find_location(server));
    }
    else
    {
        setPath("");
        setLocation(find_location(server));
        setIndex("/" + getLocation().at("index"));
    }
    if (getLocation().find("root") != getLocation().end())
        setCompletePath(getLocation().at("root") + getPath() + getIndex());
    else
    {
        if (configMap.find("root") != configMap.end())
            setCompletePath(configMap.at("root") + getPath() + getIndex());
        else
            setCompletePath("");
    }
}

void	Response::prepareResponse(std::string req, char **envp, Config config)
{
    Parser parser;
	Request	request;
    ServerConfig   server = config.getServers()[0];

    request = parser.parse(req);
	setStatus("200 OK");

	init_location(server, request);
	if (request.getMethod() == "GET" || request.getMethod() == "POST")
    	getHandler(request, 0, envp, server);
	else if (request.getMethod() == "HEAD")
		getHandler(request, 1, envp, server);
	else if (request.getMethod() == "PUT")
		putHandler(request, envp, server);
	else if (request.getMethod() == "DELETE")
		deleteHandler(request, envp, server);
}

//fonction qui donne la date au format GMT
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

const std::string &Response::getPath() const {
    return _path;
}

void Response::setPath(const std::string &path) {
    _path = path;
}

const std::string &Response::getIndex() const {
    return _index;
}

void Response::setIndex(const std::string &index) {
    _index = index;
}

const std::string &Response::getCompletePath() const {
    return _complete_path;
}

void Response::setCompletePath(const std::string &completePath) {
    _complete_path = completePath;
}

const std::map<std::string, std::string> &Response::getLocation() const
{
    return _location;
}

void Response::setLocation(const std::map<std::string, std::string> &location)
{
    _location = location;
}

std::ofstream&	operator<<(std::ofstream &o, const Response &res)
{
	o << res.stringify();
	return (o);
}
