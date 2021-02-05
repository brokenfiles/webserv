#include <lastlog.h>
#include <algorithm>
#include "Response.hpp"
#include "../config/LocationConfig.hpp"
#include "../config/ServerConfig.hpp"

Response::Response()
{
	this->addError(200, "OK", "");
	this->addError(201, "Created", "");
	this->addError(202, "Accepted", "");
	this->addError(300, "Multiple Choices", "");
	this->addError(301, "Moved Permanently", "");
	this->addError(302, "Found", "");
	this->addError(310, "Too many Redirects", "");
	this->addError(400, "Bad request", "/server/bad_request.html");
	this->addError(401, "Unauthorized", "/server/unauthorized.html");
	this->addError(403, "Forbidden", "/server/forbidden.html");
	this->addError(404, "Not found", "/server/NotFound.html");
	this->addError(500, "Internal Server Error", "/server/server_error.html");
	this->addError(501, "Not implemented", "");
	this->addError(502, "Bad Gateway", "");
	this->addError(503, "Service Unavailable", "");
}

Response::~Response()
{

}

Response::Response(const Response &copy)
{
    *this = copy;
}

Response &Response::operator= (const Response &copy)
{
	if (this != &copy)
	{
		this->_statusMessages = copy._statusMessages;
		this->_statusCode     = copy._statusCode;
	}
	return (*this);
}

/**
 *
 * @param client
 * @return
 */
std::string Response::sendResponse(Client *client)
{
	LocationConfig location = this->find_location(client);
	const std::string method = this->toLower(client->getObjRequest().getMethod());

	if (method == "get" || method == "head") {

	}
	return (this->stringify());
}

void Response::getHandler(Client *client)
{
	(void)client;
}

/**
 * Used to stringify the response.cpp class
 * Stringify the function is needed for send it to the client
 * @return stringified response
 */
std::string 	Response::stringify() const
{
	std::string string;

	string = "HTTP/1.1 " + Logger::to_string(this->getStatusCode()) + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->getHeaders().begin(); it != this->getHeaders().end(); it++)
		string += it->first + ": " + it->second + "\n";
	string += "\r\n";
	string += this->getBody();

	return (string);
}

/**
 * Récupérer la meilleure location dans le serveur
 * @param client le client
 * @return la meilleure location
 */
LocationConfig &Response::find_location(Client *client)
{
	std::list<LocationConfig> location_list = client->getServerConfig().getLocations();

	for (std::list<LocationConfig>::iterator ite = location_list.begin(); ite != location_list.end(); ite++)
	{
		if ((*ite).configuration["path"] == getDirName(client->getObjRequest().getPath()))
			return ((*ite));
	}
	for (std::list<LocationConfig>::iterator it = location_list.begin(); it != location_list.end(); it++)
	{
		if ((*it).configuration["path"] == "/")
			return ((*it));
	}
	throw NoLocationException();
}

/**
 * Récupérer la date courante pour envoyer dans les headers la date
 * @return the date
 */
std::string Response::currentDate ()
{
	struct timeval tv;
	std::string    ret;
	char           str[64];
	time_t         time;
	struct tm      *current = NULL;

	gettimeofday(&tv, NULL);
	time    = tv.tv_sec;
	current = localtime(&time);
	strftime(str, sizeof str, "%a, %d %b %G %T GMT", current);
	ret.assign(str);
	return (ret);
}

/**
 * Returns a lower string
 * @param string input string
 * @return lower string
 */
std::string Response::toLower(std::string string) {
	for (std::string::iterator it = string.begin(); it != string.end(); it++)
		*it = std::tolower(*it);
	return (string);
}

/**
 * Récupérer le path d'un fichier (exemple : /bonjour/file.html renvoie /bonjour)
 * @param file nom du fichier
 * @return path du fichier
 */
std::string Response::getDirName (const std::string& file)
{
	char separator = '/';
	size_t i = file.rfind(separator, file.length());
	if (i != std::string::npos) {
		return(file.substr(0, i));
	}
	return("");
}

const std::string &Response::getStatusCode () const
{
	return _statusCode;
}

const std::string &Response::getMessageCode (int code)
{
	return (this->_statusMessages[code].first);
}

const std::string &Response::getFileCode (int code)
{
	return (this->_statusMessages[code].second);
}

void Response::setMessageCode (int code, const std::string& message)
{
	this->_statusMessages[code].first = message;
}

void Response::setFileCode (int code, const std::string& file)
{
	this->_statusMessages[code].second = file;
}

void Response::addError (int code, const std::string &message, const std::string &file)
{
	this->setMessageCode(code, message);
	this->setFileCode(code, file);
}

const char *Response::NoLocationException::what () const throw()
{
	return "Aucune location trouvée pour le serveur";
}
