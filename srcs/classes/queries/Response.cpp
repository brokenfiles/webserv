#include "Response.hpp"
#include <algorithm>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

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
	bool foundLocation = false;
	try {
		this->_location = this->find_location(client);
		foundLocation = true;
	} catch (const std::exception &exception) {
		logger.warning("La location n'a pas été trouvée pour cette requête");
	}

	/* On définie les headers par défaut */
	this->setDefaultHeaders(client, client->getServerConfig());

	/* Gère si le serveur est unavailable */
	if (!client->isConnected() || !foundLocation)
		this->handleServerUnavailable(client);

	/* On check si la méthode est gérée par la location */
	else if (!this->isMethodValid(method)) {
		/* la méthode est invalide */
		this->_statusCode = this->getMessageCode(405);
		this->_headers["Allow"] = this->_location.getRawMethods();
	} else {
		// handle max body size
		if (this->_location.getMaxBodySize() != DEFAULT_MAX_BODY_SIZE) {
			if ((int)client->getObjRequest().getBody().size() > this->_location.getMaxBodySize()) {
				this->_statusCode = getMessageCode(413);
			}
		}
		// on vérifie que le status code est à 200
		if (this->_statusCode == getMessageCode(200)) {
			if (!Cgi::isCGI(client->getObjRequest(), this->_location)) {
				logger.info("Executing built-in methods for this request");
				// on regarde si auth est empty ou non dans la location
				if (!this->_location.getAuth().empty()) {
					bool authenticated = this->authenticate(client);
					if (!authenticated) {
						this->_statusCode = getMessageCode(401);
						this->_headers["WWW-Authenticate"] = "Basic";
					}
				}
				if (this->_statusCode == getMessageCode(200)) {
					/* la méthode est valide */
					if (method == "get" || method == "head") {
						this->getHandler(client);
						if (method == "head")
							this->setBody("");
					} else if (method == "put" || method == "patch") {
						this->putHandler(client);
					} else if (method == "options") {
						this->optionsHandler();
					} else if (method == "connect") {
						// connexion établie, rien à faire
					} else if (method == "trace") {
						this->traceHandler(client);
					} else if (method == "post") {
						this->postHandler(client);
					} else if (method == "delete") {
						this->deleteHandler(client);
					} else {
						// la méthode n'est pas géré par webserv
						this->_statusCode = getMessageCode(405);
					}
				}
			} else {
				logger.info("Executing CGI for this request");
				Cgi cgi;
				// execute CGIs
				cgi.launch(client, *this);
			}
		}
	}

	// display error codes
	// si le premier char de l'erreur n'est pas égal à 2, c'est une erreur : afficher l'erreur
	if (this->_statusCode.find("200") == std::string::npos && this->getBody().empty()) {
		this->displayErrors();
	}

	return (this->stringify());
}

/**
 * Mets un status code 503 si le serveur est connected
 * @param client
 */
void Response::handleServerUnavailable (Client *client)
{
	if (!client->isConnected()) {
		this->_statusCode = getMessageCode(503);
		this->_headers["Retry-After"] = "120";
	}
}

/**
 * Fonction qui gère les reauêtes GET
 * @param client
 */
void Response::getHandler(Client *client)
{
	std::string requestFile = client->getObjRequest().getDefaultPath(this->_location);
	this->handleAcceptCharset(client);
	if (this->_statusCode != getMessageCode(200))
		return ;
	// on gère le accept language et on ajoute "extLanguage" à request file
	this->handleAcceptLanguage(client, requestFile);
	requestFile += this->_extLanguage;
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
			this->_headers["Content-Location"] = requestFile;
			this->_headers["Last-Modified"] = getLastModified(requestFile);
		} else {
			// le fichier est inaccessible on retourne une erreur 403 forbidden
			this->_statusCode = getMessageCode(403);
		}
	} else {
		if (this->_location.getAutoindex() == "on") {
			// on essaie le directory listing
			this->tryDirectoryListing(
					_location.getRootDir() + Request::getPathWithoutLocation(client->getObjRequest().getPath(),
															  this->_location), client);
		} else {
			// il n'existe pas on retourne une erreur 404 not found
			this->_statusCode = getMessageCode(404);
		}
	}
}

void Response::putHandler(Client *client)
{
	std::string requestFile = Request::getPathWithIndex(this->_location.getUploadDir() + client->getObjRequest().getPath(), this->_location);
	bool fileExists = std::ifstream(requestFile.c_str()).good();
	// on ouvre in filestream
    std::ofstream fileStream(requestFile.c_str());
	// on regarde si le fichier existe
	if (fileStream.is_open()) {
		// il existe
		fileStream << client->getObjRequest().getBody();
		// on retourne un 200 si le fichier existait avant sinon un 201
		this->_statusCode = getMessageCode(fileExists ? 200 : 201);
		fileStream.close();
		if (!fileExists)
			this->_headers["Location"] = client->getObjRequest().getPath();
		this->_headers["Content-Location"] = requestFile;
		this->_headers["Last-Modified"] = getLastModified(requestFile);
	} else {
		// il n'exite pas on retourne une erreur 403
		this->_statusCode = getMessageCode(403);
	}
}

/**
 * Cette méthode affiche les Options de la location (exemple : Allow)
 * @param client
 */
void Response::optionsHandler()
{
	this->_headers["Allow"] = this->_location.getRawMethods();
}

void Response::traceHandler(Client *client)
{
	std::string headers;

	headers += client->getObjRequest().getMethod() + " " + client->getObjRequest().getPath() + " HTTP/1.1\n";
	std::map<std::string, std::string>::const_iterator it = client->getObjRequest().getHeaders().begin();
	while (it != client->getObjRequest().getHeaders().end()) {
		headers += it->first + ": " + it->second + "\n";
		it ++;
	}
	this->setBody(headers);
	this->_headers["Content-Type"] = "message/http";
}

void Response::postHandler(Client *client)
{
	std::string requestFile = Request::getPathWithIndex(this->_location.getUploadDir() + client->getObjRequest().getPath(), this->_location);
	int fd;
	struct stat sb;
	bool fileExists = stat(requestFile.c_str(), &sb) != -1;
	// on ouvre le fichier (on le créé si il n'existe pas ou on l'ouvre en concat)
	if ((fd = open(requestFile.c_str(), (fileExists ? O_APPEND : O_CREAT) | O_WRONLY, 0666)) == -1) {
		this->_statusCode = getMessageCode(500);
	} else {
		// on écrit dans le fichier
		int ret = write(fd, client->getObjRequest().getBody().c_str(), client->getObjRequest().getBody().size());
		logger.info("(Post Request) - File written (path : " + requestFile + ") - Write return : " + Logger::to_string(ret));
		// si ret est <= 0, il y a eu une erreur, on retourne une erreur 500
		if (ret < 0)
			this->_statusCode = getMessageCode(500);
		else {
			this->_headers["Content-Location"] = requestFile;
			this->_statusCode = getMessageCode(fileExists ? 200 : 201);
			if (this->_statusCode == getMessageCode(200)) {
				this->setBody("File updated.");
				this->_headers["Last-Modified"] = getLastModified(requestFile);
			} else if (this->_statusCode == getMessageCode(201)) {
				this->_headers["Location"] = client->getObjRequest().getPath();
			}
		}
		close(fd);
	}
}

void Response::deleteHandler(Client *client)
{
	std::string requestFile = this->_location.getUploadDir() + client->getObjRequest().getPath();
	struct stat sb;
	bool fileExists = stat(requestFile.c_str(), &sb) != -1;

	if (!fileExists) {
		this->_statusCode = getMessageCode(404);
	} else {
		if (S_ISDIR(sb.st_mode)) {
			// on regarde si le dossier que l'utilisateur veut delete ne correspond pas au dossier d'upload
			std::string uploadDir = std::string(this->_location.getUploadDir());
			if (uploadDir[uploadDir.size() - 1] != '/') {
				uploadDir += '/';
			}
			if (uploadDir != requestFile) {
				try {
					this->_headers["Content-Location"] = requestFile;
					this->removeDir(requestFile, client);
					this->setBody("Directory deleted.");
				} catch (const std::exception &e) {
					this->_statusCode = getMessageCode(500);
				}
			} else {
				// le dossier que l'user veut delete est le dossier d'upload
				// on renvoit une bad request
				this->_statusCode = getMessageCode(400);
			}
		} else {
			if (std::remove(requestFile.c_str()) == 0) {
				this->_statusCode = getMessageCode(200);
				this->setBody("File deleted.");
			} else {
				this->_statusCode = getMessageCode(500);
			}
		}
	}
}

/**
 * Cette fonction prends le client, récupère le token d'authentification, le décode et compare avec la valeur auth
 * @param client
 * @return si l'authentification est bonne
 */
bool Response::authenticate(Client *client)
{
	if (client->getObjRequest().getHeaders().find("Authorization") == client->getObjRequest().getHeaders().end())
		return false;
	std::string authorization = client->getObjRequest().getHeaders().at("Authorization");
	std::vector<std::string> authSpecs = Utils::explode(authorization, " ");
	if (authSpecs.size() == 2) {
		if (toLower(authSpecs[0]) == "basic") {
			std::string decoded = Utils::decodeBase64(authSpecs[1]);
			if (decoded == this->_location.getAuth()) {
				logger.success("Client authenticated");
				return true;
			} else {
				logger.error("Identifiers tried : " + decoded);
			}
		}
	}
	logger.error("Client failed to authenticate");
	return false;
}

/**
 * Cette fonction supprime un directory
 * En vérifiant que le directory ne correspond pas au upload_dir
 * @param path
 * @param client
 */
void Response::removeDir(const std::string &path, Client *client)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			std::string entityName = std::string(ent->d_name);
			if (entityName == "." || entityName == "..") {
				continue;
			}
			std::string pathToEntity = client->getObjRequest().getPath();
			if (pathToEntity[pathToEntity.size() - 1] != '/')
				pathToEntity += '/';
			pathToEntity += entityName;
			if (ent->d_type == DT_DIR)
				this->removeDir(path + "/" + entityName, client);
			else
				unlink(std::string(path + "/" + entityName).c_str());
		}
		rmdir(path.c_str());
		closedir (dir);
	} else {
		/* could not open directory */
		throw CantOpenDirectoryException();
	}
}

/**
 * Cette fonction gère le header "Accept-Language"
 * Les fichiers peuvent avoir une extension .<lang>
 * Si cette langue est spécifiée, ce sera ce fichier qui sera renvoyé
 * @param client
 * @param requestFile
 */
void Response::handleAcceptLanguage(Client *client, const std::string &requestFile)
{
	// on check si le header accept-language existe
	if (client->getObjRequest().getHeaders().find("Accept-Language") == client->getObjRequest().getHeaders().end())
		return ;
	std::map<std::string, std::string> headers = client->getObjRequest().getHeaders();
	struct stat fileStat;
	// on récupère les langages en raw pour les split plus tard
	std::string rawLanguages = headers["Accept-Language"];
	// on split les langages
	std::vector<std::string> languages = Utils::explode(rawLanguages, ",");
	// si languages.size == 1 ça veut dire qu'il n'y en a qu'un seul
	// sinon il y a plusieurs langages, on doit le gérer
	if (languages.size() == 1) {
		std::string lang = Utils::removeWhitespaces(languages.front());
		// si la langue n'est pas égale à un wildcard (toute langue autorisée)
		if (lang != "*") {
			if (stat(std::string(requestFile + "." + lang).c_str(), &fileStat) != -1) {
				this->_extLanguage = "." + lang;
				this->_headers["Content-Language"] = lang;
			}
		}
	} else if (languages.size() > 1) {
		std::vector<std::string>::iterator it = languages.begin();
		std::list<std::string> orderedLanguages;
		while (it != languages.end()) {
			std::string temporaryLang = Utils::removeWhitespaces(*it);
			std::vector<std::string> langSpecs = Utils::explode(temporaryLang, ";");
			// on ajoute les languages au fur et à mesure de la lecture dans une liste
			orderedLanguages.push_back(Utils::removeWhitespaces(langSpecs.front()));
			it ++;
		}
		// on dispose de la liste et on va pouvoir choisir le meilleur langage
		std::list<std::string>::iterator listIt = orderedLanguages.begin();
		while (listIt != orderedLanguages.end()) {
			if (stat(std::string(requestFile + "." + *listIt).c_str(), &fileStat) != -1) {
				this->_extLanguage = "." + *listIt;
				this->_headers["Content-Language"] = *listIt;
				break ;
			}
			listIt ++;
		}
	}
	if (!this->_extLanguage.empty()) {
		logger.success("File found for wanted language. Extension : " + this->_extLanguage);
	}
}

/**
 * Cette fonction regarde si le charset de la requête est géré par webserv
 * S'il est géré, on ne fait rien
 * Sinon, on renvoie un code 406 non acceptable
 * @param client
 */
void Response::handleAcceptCharset(Client *client)
{
	// on check si le header accept-charset existe
	if (client->getObjRequest().getHeaders().find("Accept-Charset") == client->getObjRequest().getHeaders().end())
		return ;
	std::map<std::string, std::string> headers = client->getObjRequest().getHeaders();
	// on récupère les langages en raw pour les split plus tard
	std::string rawCharsets = headers["Accept-Charset"];
	// on split les charsets
	std::vector<std::string> charsets = Utils::explode(rawCharsets, ",");
	// si charsets.size == 1 ça veut dire qu'il n'y en a qu'un seul
	// sinon il y a plusieurs charsets, on doit le gérer
	if (charsets.size() == 1) {
		std::string charset = this->toLower(Utils::removeWhitespaces(charsets.front()));
		// si la langue n'est pas égale à un wildcard (toute langue autorisée)
		if (charset == "*" || charset == "utf-8") {
			return ;
		}
	} else if (charsets.size() > 1) {
		std::vector<std::string>::iterator it = charsets.begin();
		std::list<std::string> orderedCharsets;
		while (it != charsets.end()) {
			std::string temporaryCharset = Utils::removeWhitespaces(*it);
			std::vector<std::string> charsetSpecs = Utils::explode(temporaryCharset, ";");
			// on ajoute les languages au fur et à mesure de la lecture dans une liste
			orderedCharsets.push_back(this->toLower(Utils::removeWhitespaces(charsetSpecs.front())));
			it ++;
		}
		// on dispose de la liste et on va pouvoir choisir le meilleur langage
		std::list<std::string>::iterator listIt = orderedCharsets.begin();
		while (listIt != orderedCharsets.end()) {
			if (*listIt == "*" || *listIt == "utf-8")
				return ;
			listIt ++;
		}
	}
	this->_statusCode = getMessageCode(406);
	logger.error("Client accept-charset is invalid (the charset in not handled by server)");
}

void Response::tryDirectoryListing (const std::string &path, Client *client) {
	try {
		std::string files = this->getFilesInDirectory(path, client);
		std::ifstream fileStream("www/server/DirectoryListingTemplate.html", std::ifstream::in);
		// on regarde si le fichier existe
		if (fileStream.good())
		{
			// il existe
			if (fileStream.is_open())
			{
				// on peut lire le fichier, on l'ajoute au body
				// pour lire des gros fichiers avec buffer : http://www.cplusplus.com/reference/istream/istream/read/
				std::string fileContent( (std::istreambuf_iterator<char>(fileStream) ), (std::istreambuf_iterator<char>()));

				Utils::replaceString(fileContent, "${DIRECTORY_NAME}", client->getObjRequest().getPath());
				Utils::replaceString(fileContent, "${FILES}", files);

				this->setBody(fileContent);
				fileStream.close();
			}
		}
	} catch (const std::exception &exception) {
		logger.warning("(DIRECTORY LISTING ERROR!) " + Logger::to_string(exception.what()));
		this->_statusCode = getMessageCode(404);
	}
}

std::string Response::getFilesInDirectory(const std::string &path, Client *client)
{
	std::string HTML_files;
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			std::string entityName = std::string(ent->d_name);
			if (entityName == "." || entityName == "..") {
				continue;
			}
			std::string pathToEntity = client->getObjRequest().getPath();
			if (pathToEntity[pathToEntity.size() - 1] != '/')
				pathToEntity += '/';
			pathToEntity += entityName;
			std::string lastModified = getLastModified(path + "/" + entityName);
			HTML_files += "<div class=\"link\"><p class=\"path\"><a href=\"" + pathToEntity +"\">" + entityName + "</a><p>" + lastModified + "</p></div>\n";
		}
		closedir (dir);
	} else {
		/* could not open directory */
		throw CantOpenDirectoryException();
	}
	return HTML_files;
}

/**
 * Stringify les headers
 * @return les headers stringifiés
 */
std::string	Response::stringifyHeaders() const
{
	std::string headers;

	headers = "HTTP/1.1 " + Logger::to_string(this->getStatusCode()) + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->getHeaders().begin(); it != this->getHeaders().end(); it++)
		headers += it->first + ": " + it->second + "\r\n";
	headers += getCookies();
	headers += "\r\n";
	return headers;
}

/**
 * Used to stringify the response.cpp class
 * Stringify the function is needed for send it to the client
 * @return stringified response
 */
std::string 	Response::stringify() const
{
	std::string string;

	string += this->stringifyHeaders();
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
	this->_headers["Host"] = client->getServerConfig().getServerName() + ":" + Logger::to_string(client->getServerConfig().getPort());
	this->_headers["Server"] = "Webserv/1.0 (Ubuntu)";
	this->_headers["Content-Length"] = "0";
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
LocationConfig Response::find_location(Client *client)
{
	std::list<LocationConfig> matchedLocations;
	for (std::list<LocationConfig>::iterator it = client->getServerConfig().getLocations().begin(); it != client->getServerConfig().getLocations().end(); it++)
	{
		if (this->getPathWithSlash((*it).getPath()) == this->getPathWithSlash(getDirName(client->getObjRequest().getPath()))) {
			matchedLocations.push_back(*it);
		}
	}
	if (matchedLocations.empty()) {
		for (std::list<LocationConfig>::iterator it1 = client->getServerConfig().getLocations().begin(); it1 != client->getServerConfig().getLocations().end(); it1++)
		{
			if ((*it1).getPath() == "/")
				return ((*it1));
		}
	} else {
		std::string path = client->getObjRequest().getDefaultPath(this->_location);
		size_t index = path.rfind('.');
		if (index == std::string::npos)
			return (matchedLocations.front());
		std::string reqExtension = path.substr(index, path.size() - path.rfind('.'));
		std::list<LocationConfig>::iterator matchedLocationsIterator = matchedLocations.begin();
		while (matchedLocationsIterator != matchedLocations.end()) {
			if (matchedLocationsIterator->getCgiExtension() == reqExtension) {
				logger.info("Found best location for extension " + reqExtension);
				return *matchedLocationsIterator;
			}
			matchedLocationsIterator ++;
		}
		return (matchedLocations.front());
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
	if (path.size() == 0) {
		return (path += '/');
	}
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

std::string Response::getLastModified (const std::string &file)
{
	struct stat info;
	std::string sec;
	struct tm   time;
	std::string date;
	char        buffer[1000];

	stat(file.c_str(), &info);
	sec = Logger::to_string(info.st_mtime);
	strptime(sec.c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S CEST", &time);
	date = buffer;
	return (date);
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

/**
 * Cette fonction afficher les erreurs grâce à la map statusMessages et au statusCode
 */
void Response::displayErrors ()
{
	std::map<int, std::string>::iterator it = this->_statusMessages.begin();
	while (it != this->_statusMessages.end())
	{
		if (Logger::to_string(it->first) + " " + it->second == this->_statusCode)
		{
			std::ifstream fileStream("www/server/ErrorTemplate.html", std::ifstream::in);
			// on regarde si le fichier existe
			if (fileStream.good())
			{
				// il existe
				if (fileStream.is_open())
				{
					// on peut lire le fichier, on l'ajoute au body
					// pour lire des gros fichiers avec buffer : http://www.cplusplus.com/reference/istream/istream/read/
					std::string fileContent( (std::istreambuf_iterator<char>(fileStream) ), (std::istreambuf_iterator<char>()));

					Utils::replaceString(fileContent, "ErrorStatus", it->second);
					Utils::replaceString(fileContent, "ErrorCode", Logger::to_string(it->first));

					this->setBody(fileContent);
					fileStream.close();
				}
			}
		}
		it++;
	}
}

/**
 * Getter pour le statusCode
 * @return le statusCode
 */
const std::string &Response::getStatusCode () const
{
	return _statusCode;
}

void Response::setDefaultStatusCodes()
{
	this->addError(200, "OK");
	this->addError(201, "Created");
	this->addError(202, "Accepted");
	this->addError(204, "No Content");
	this->addError(300, "Multiple Choices");
	this->addError(301, "Moved Permanently");
	this->addError(302, "Found");
	this->addError(310, "Too many Redirects");
	this->addError(400, "Bad request");
	this->addError(401, "Unauthorized");
	this->addError(403, "Forbidden");
	this->addError(404, "Not Found");
	this->addError(405, "Method Not Allowed");
	this->addError(406, "Non acceptable");
	this->addError(413, "Request Entity Too Large");
	this->addError(500, "Internal Server Error");
	this->addError(501, "Not Implemented");
	this->addError(502, "Bad Gateway");
	this->addError(503, "Service Unavailable");
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
	return (Logger::to_string(code) + " " + this->_statusMessages[code]);
}

void Response::setMessageCode (int code, const std::string& message)
{
	this->_statusMessages[code] = message;
}

void Response::addError (int code, const std::string &message)
{
	this->setMessageCode(code, message);
}

std::map<int, std::string > &Response::getStatusMessages ()
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

void Response::setStatusCode(const std::string &statusCode)
{
	_statusCode = statusCode;
}

const char *Response::NoLocationException::what () const throw()
{
	return "Aucune location trouvée pour le serveur";
}

const char *Response::CantOpenDirectoryException::what () const throw()
{
	return "Impossible d'ouvrir ce directory";
}
