#include <algorithm>
#include <sys/time.h>
#include "Response.hpp"
#include "../config/LocationConfig.hpp"
#include "../config/ServerConfig.hpp"

Response::Response()
{
	this->setDefaultStatusCodes();
	this->setDefaultExtensions();
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
	const std::string method = this->toLower(client->getObjRequest().getMethod());
	this->_location = this->find_location(client);

	/* On définie les headers par défaut */
	this->setDefaultHeaders(client, client->getServerConfig());

	/* On check si la méthode est gérée par la location */
	if (!this->isMethodValid(method)) {
		/* la méthode est invalide */
		this->_statusCode = this->getMessageCode(401);
	} else {
		if (!Cgi::isCGI(client->getObjRequest(), this->_location)) {
			/* la méthode est valide */
			if (method == "get" || method == "head") {
				this->getHandler(client);
				if (method == "head")
					this->setBody("");
			} else if (method == "put") {
				this->putHandler(client);
			} else if (method == "delete") {
				this->deleteHandler(client);
			}
		} else {
			Cgi cgi;
			// execute CGIs
			cgi.execute(client, *this);
		}
	}
	// display error codes
	// si le premier char de l'erreur n'est pas égal à 2, c'est une erreur : afficher l'erreur
	if (this->_statusCode.find("200") == std::string::npos) {
		this->displayErrors();
	}

	return (this->stringify());
}

/**
 * Fonction qui gère les reauêtes GET
 * @param client
 */
void Response::getHandler(Client *client)
{
	std::string requestFile = client->getObjRequest().getDefaultPath(this->_location);
	// on ouvre in filestream
	std::ifstream fileStream(requestFile.c_str(), std::ifstream::in);
	// on regarde si le fichier existe
	if (fileStream.good()) {
		// il existe
		if (fileStream.is_open()) {
			// on peut lire le fichier, on l'ajoute au body
			// pour lire des gros fichiers avec buffer : http://www.cplusplus.com/reference/istream/istream/read/
			std::string fileContent( (std::istreambuf_iterator<char>(fileStream) ),
								 (std::istreambuf_iterator<char>()    ) );

			this->setBody(fileContent);
			fileStream.close();
		} else {
			// le fichier est inaccessible on retourne une erreur 403 forbidden
			this->_statusCode = getMessageCode(403);
		}
	} else {
		// il n'exite pas on retourne une erreur 404 not found
		this->_statusCode = getMessageCode(404);
	}
}

void Response::putHandler(Client *client)
{
	std::string requestFile = this->_location.getUploadDir() + client->getObjRequest().getPath();
	bool fileExists = std::ifstream(requestFile.c_str()).good();
	// on ouvre in filestream
	std::ofstream fileStream(requestFile.c_str());
	// on regarde si le fichier existe
	if (fileStream.is_open()) {
		// il existe
		fileStream << client->getObjRequest().getBody();
		// on retourne un 204 si le fichier existait avant sinon un 201
		this->_statusCode = getMessageCode(fileExists ? 204 : 201);
	} else {
		// il n'exite pas on retourne une erreur 401 unauthorized
		this->_statusCode = getMessageCode(403);
	}
}

void Response::deleteHandler(Client *client)
{
	std::string requestFile = this->_location.getUploadDir() + client->getObjRequest().getPath();
	if (std::remove(requestFile.c_str()) == 0) {

	}
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
 * Mets les headers à leur état initial et définie le status code à 200 par défaut
 * @param client
 * @param server la configuration du serveur
 */
void Response::setDefaultHeaders(Client *client, ServerConfig &server)
{
	(void)server;
	this->_statusCode = this->getMessageCode(200);
	this->setContentType(client);
	this->_headers["Date"] = this->currentDate();
	this->_headers["Server"] = "Webserv";
}

/**
 * Définie le content type par rapport à l'extension du fichier
 * @param client
 */
void Response::setContentType(Client *client)
{
	std::string path = client->getObjRequest().getDefaultPath(this->_location);
	size_t index = path.rfind('.');
	if (index == std::string::npos)
		index = 0;
	std::string reqExtension = path.substr(index, path.size() - path.rfind('.'));

	if (this->_contentTypes.find(reqExtension) != this->_contentTypes.end()) {
		this->_headers["Content-Type"] = this->_contentTypes[reqExtension];
	} else {
		this->_headers["Content-Type"] = "text/html";
	}
}

/**
 * Récupérer la meilleure location dans le serveur
 * @param client le client
 * @return la meilleure location
 */
LocationConfig &Response::find_location(Client *client)
{
	for (std::list<LocationConfig>::iterator ite = client->getServerConfig().getLocations().begin(); ite != client->getServerConfig().getLocations().end(); ite++)
	{
		if (this->getPathWithSlash((*ite).getPath()) == this->getPathWithSlash(getDirName(client->getObjRequest().getPath())))
			return ((*ite));
	}
	for (std::list<LocationConfig>::iterator it = client->getServerConfig().getLocations().begin(); it != client->getServerConfig().getLocations().end(); it++)
	{
		if ((*it).getPath() == "/")
			return ((*it));
	}
	throw NoLocationException();
}

/**
 * On check si la méthode est bien autorisée dans la location
 * @param method la méthode
 * @return vrai si la méthode est dans la location, non autrement
 */
bool Response::isMethodValid(const std::string &method)
{
	std::vector<std::string> methods = this->_location.getMethods();

	for(std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++) {
		if (this->toLower(*it) == this->toLower(method)) {
			return (true);
		}
	}
	return (false);
}

/**
 * retourne le chemin avec un slash
 * @param path
 * @return
 */
std::string Response::getPathWithSlash(std::string path) {
	if (path.size() > 0 && path[path.size() - 1] != '/') {
		return (path += '/');
	} else {
		return (path);
	}
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
	std::string fileSlash = file;
	if (file.find('.') == std::string::npos) {
		fileSlash = this->getPathWithSlash(file);
	}
	size_t firstSlash = fileSlash.find(separator);
	size_t secondSlash = fileSlash.find(separator, 1);
	if (firstSlash == std::string::npos || secondSlash == std::string::npos) {
		return ("");
	}
	return(fileSlash.substr(firstSlash, secondSlash - firstSlash));
}

void Response::replace(std::string &fileContent, std::string replace, std::string newString)
{
    size_t pos = 0;

    while ((pos = fileContent.find(replace)) != std::string::npos)
    {
        fileContent.replace(pos, replace.length(), newString);
        pos += newString.length();
    }
}

void Response::displayErrors ()
{
	std::map<int, std::pair<std::string, std::string> >::iterator begin = this->_statusMessages.begin();
	while (begin != this->_statusMessages.end())
	{
		if (Logger::to_string(begin->first) + " " + begin->second.first == this->_statusCode)
		{
			std::ifstream fileStream("srcs/home/server/ErrorTemplate.html", std::ifstream::in);
			// on regarde si le fichier existe
			if (fileStream.good())
			{
				// il existe
				if (fileStream.is_open())
				{
					// on peut lire le fichier, on l'ajoute au body
					// pour lire des gros fichiers avec buffer : http://www.cplusplus.com/reference/istream/istream/read/
					std::string fileContent( (std::istreambuf_iterator<char>(fileStream) ), (std::istreambuf_iterator<char>()));

					this->replace(fileContent, "ErrorStatus", begin->second.first);
					this->replace(fileContent, "ErrorCode", logger.to_string(begin->first));

					this->setBody(fileContent);
					fileStream.close();
				}
			}
		}
		begin++;
	}
}

const std::string &Response::getStatusCode () const
{
	return _statusCode;
}

void Response::setDefaultStatusCodes()
{
	this->addError(200, "OK", "");
	this->addError(201, "Created", "");
	this->addError(202, "Accepted", "");
	this->addError(204, "No Content", "");
	this->addError(300, "Multiple Choices", "");
	this->addError(301, "Moved Permanently", "");
	this->addError(302, "Found", "");
	this->addError(310, "Too many Redirects", "");
	this->addError(400, "Bad request", "srcs/home/server/bad_request.html");
	this->addError(401, "Unauthorized", "srcs/home/server/unauthorized.html");
	this->addError(403, "Forbidden", "srcs/home/server/forbidden.html");
	this->addError(404, "Not Found", "srcs/home/server/NotFound.html");
	this->addError(413, "Request Entity Too Large", "srcs/home/server/NotFound.html");
	this->addError(500, "Internal Server Error", "srcs/home/server/server_error.html");
	this->addError(501, "Not Implemented", "");
	this->addError(502, "Bad Gateway", "");
	this->addError(503, "Service Unavailable", "");
}

void Response::setDefaultExtensions()
{
	this->_contentTypes[".html"] = "text/html";
	this->_contentTypes[".htm"] = "text/html";
	this->_contentTypes[".php"] = "text/html";
	this->_contentTypes[".bla"] = "text/html";
	this->_contentTypes[".css"] = "text/css";
	this->_contentTypes[".gif"] = "image/gif";
	this->_contentTypes[".ico"] = "image/x-icon";
	this->_contentTypes[".jpeg"] = "image/jpeg";
	this->_contentTypes[".jpg"] = "image/jpeg";
	this->_contentTypes[".js"] = "application/javascript";
	this->_contentTypes[".json"] = "application/json";
	this->_contentTypes[".png"] = "image/png";
	this->_contentTypes[".pdf"] = "application/pdf";
	this->_contentTypes[".svg"] = "image/svg+xml";
	this->_contentTypes[".woff"] = "font/woff";
	this->_contentTypes[".woff2"] = "font/woff2";
	this->_contentTypes[".xhtml"] = "application/xhtml+xml";
}

std::string Response::getMessageCode (int code)
{
	return (Logger::to_string(code) + " " + this->_statusMessages[code].first);
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

std::map<int, std::pair<std::string, std::string> > &Response::getStatusMessages ()
{
	return _statusMessages;
}

std::map<std::string, std::string> &Response::getContentTypes ()
{
	return _contentTypes;
}

LocationConfig &Response::getLocation ()
{
	return _location;
}

const char *Response::NoLocationException::what () const throw()
{
	return "Aucune location trouvée pour le serveur";
}
