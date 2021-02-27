//
// Created by Timothe Lecouvreur on 11/01/2021.
//

#include "Cgi.hpp"
#include "../../../includes/utils.hpp"
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

Cgi::Cgi()
{}

Cgi::~Cgi()
{

}

/**
 * Cette fonction execute le cgi avec le binaire correspondant
 * @param client
 * @param response
 * @action remplace le body de la reponse par le retour du CGI
 */
void	Cgi::launch(Client *client, Response &response)
{
    //on set le nom du fichier qui doit etre executé dans le cgi
	setRequestFile(client->getObjRequest().getDefaultPath(const_cast<LocationConfig &>(response.getLocation())));

	//si le fichier demandé est un .bla on execute le cgi meme si il n'existe pas
	if (response.getLocation().getCgiExtension() != ".bla")
	{
		// on ouvre in filestream
		std::ifstream fileStream(getRequestFile().c_str());

		// on regarde si le fichier existe
		if (!fileStream.is_open())
		{
			// il n'exite pas on retourne une erreur 404
			response.setStatusCode(response.getMessageCode(404));
			return;
		}
	}

    //on ajoute les META-VARIABLES
    addMetaVariables(response, client);

    //on ajoute le nom du binaire et celui du fichier dans argv, qui sera passé au CGI dans un execve
    addArgv(response);

    this->_client = client;
    this->_requestBody = client->getObjRequest().getBody();

    //on execute le CGI
    execute(response);

	//on remplace le body de la reponse par le retour du CGI
    response.setBody(this->_var.output);
}

void	Cgi::execute(Response &response)
{
	this->_var.save_in = dup(STDIN_FILENO);
	this->_var.save_out = dup(STDOUT_FILENO);

	std::string token = Utils::generateToken(16);
	std::string fileNameInput = token + ".input";
	std::string fileNameOutput = token + ".output";
	//on fork le processus en 2
	this->_var.pid = fork();
	if (this->_var.pid == 0)
	{
		this->_var.input_fd = open(fileNameInput.c_str(), O_CREAT | O_RDWR, 0777);
		this->_var.output_fd = open(fileNameOutput.c_str(), O_CREAT | O_RDWR, 0777);
		//processus fils
		dup2(this->_var.input_fd, STDIN_FILENO);
		dup2(this->_var.output_fd, STDOUT_FILENO);

		close(this->_var.input_fd);
		close(this->_var.output_fd);
		//on execute le CGI
		execve(this->_var.argv[0], this->_var.argv, this->_var.metaVarArray);
	}
	else if (this->_var.pid > 0)
	{
		this->_var.input_fd = open(fileNameInput.c_str(), O_CREAT | O_RDWR, 0777);
		this->_var.output_fd = open(fileNameOutput.c_str(), O_CREAT | O_RDWR, 0777);
		//processus pere

		//si c'est un requete POST on ecrit le body sur STDIN du processus fils
		if (this->_client->getObjRequest().getMethod() != "GET")
			write(this->_var.input_fd, this->_requestBody.c_str(), this->_requestBody.size());

		waitpid(this->_var.pid, &this->_var.status, 0);

		//on lit le retour du CGI et on le stock dans var.output
		while ((this->_var.ret = read(this->_var.output_fd, this->_var.buffer, BUFFER - 1)) != 0)
		{
			this->_var.buffer[this->_var.ret] = 0;
			this->_var.output += this->_var.buffer;
		}

		//on récupère le code de retour du CGI et des headers
		if (response.getLocation().getCgiExtension() != ".bla" || this->_client->getObjRequest().getMethod() != "GET")
			getCGIReturn(response);

		//on remet STDIN et STDOUT sur leurs fd respectifs
		dup2(this->_var.save_in, STDIN_FILENO);
		dup2(this->_var.save_out, STDOUT_FILENO);
		close(this->_var.save_in);
		close(this->_var.save_out);

		//on free l'array de variables d'environement
		for (size_t i = 0; i < this->_metaVarMap.size(); ++i) {
			free(this->_var.metaVarArray[i]);
		}
		free(this->_var.metaVarArray);
        delete[] this->_var.argv;
		close(this->_var.input_fd);
		close(this->_var.output_fd);
		remove(fileNameInput.c_str());
		remove(fileNameOutput.c_str());

	}
}

/**
 * cette fonction rempli un array avec le nom du binaire CGI a utiliser
 * et le path du fichier a executer
 * @param response : contient le nom du binaire dans une locationConfig
 */
void Cgi::addArgv(Response &response)
{
    std::vector<std::string>        vec;

    vec.push_back(response.getLocation().getCgiBin());
    vec.push_back(getRequestFile());
    this->_argv = vec;
	this->_var.argv = vecToArray(this->_argv);
}

/**
 * cette fonction récupère le status code et les headers retournés par le programme CGI et les stock dans la réponse passée en argument
 * @param response
 */
void	Cgi::getCGIReturn(Response &response)
{
	Parser parser;
	int count = 0;
	unsigned int nPos  = 0;
	std::string tmp = this->_var.output.substr(0, this->_var.output.find("\r\n\r\n", 0));
	while((unsigned int)(nPos = this->_var.output.find("\r\n", nPos)) != (unsigned int)std::string::npos) {
		count++; nPos += 2;
	}
	if (count > 1) {
		this->_var.output.erase(0, this->_var.output.find("\r\n\r\n") + 4);
	}
	Request query = parser.parseResponse(tmp + "\r\n");

	response.setCookies(query.getRawCookies());
	if (!query.getHeaders().empty()) {
		std::map<std::string, std::string>::const_iterator begin = query.getHeaders().begin();
		std::map<std::string, std::string> headers = response.getHeaders();

		for (; begin != query.getHeaders().end(); begin++)
		{
			if (begin->first == "Status")
				response.setStatusCode(query.getHeaders().at("Status"));
			else
				headers[begin->first] = begin->second;
		}
		response.setHeaders(headers);
	}
}

/**
 * Cette fonction definie les META-VARIABLES requises par le cgi pour fonctionner,
 * elles sont stockées dans une map de la classe CGI
 * @param request : la requete faite par le client
 */
void Cgi::addMetaVariables(Response &response, Client *client)
{
	Request request = client->getObjRequest();

	this->_metaVarMap["AUTH_TYPE"] = "NULL";
	this->_metaVarMap["REMOTE_ADDR"] = client->getIP();
	this->_metaVarMap["REQUEST_METHOD"] = request.getMethod();
	this->_metaVarMap["CONTENT_LENGTH"] = Logger::to_string(request.getBody().size());
    if (request.getHeaders().find("Content-Type") != request.getHeaders().end())
		this->_metaVarMap["CONTENT_TYPE"] = request.getHeaders().find("Content-Type")->second;
	this->_metaVarMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_metaVarMap["PATH_INFO"] = request.getPath();
	this->_metaVarMap["PATH_TRANSLATED"] = getRequestFile();
	if (client->getObjRequest().getQueryString().empty())
	    this->_metaVarMap["QUERY_STRING"] = "";
	else
	    this->_metaVarMap["QUERY_STRING"] = request.getQueryString();
	this->_metaVarMap["SCRIPT_FILENAME"] = getRequestFile();
	this->_metaVarMap["REQUEST_URI"] = request.getPath();
	if (!request.getQueryString().empty())
		this->_metaVarMap["REQUEST_URI"] += "?" + request.getQueryString();
	this->_metaVarMap["SCRIPT_NAME"] = response.getLocation().getCgiBin();
	this->_metaVarMap["SERVER_NAME"] = client->getServerConfig().getServerName();
	this->_metaVarMap["SERVER_PORT"] = Logger::to_string(client->getServerConfig().getPort());
	this->_metaVarMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_metaVarMap["SERVER_SOFTWARE"] = "Webserv/1.0";
	if (request.getHeaders().find("Cookie") != request.getHeaders().end())
		this->_metaVarMap["HTTP_COOKIE"] = client->getObjRequest().getHeaders().at("Cookie");
    if (request.getHeaders().find("Accept") != request.getHeaders().end())
        this->_metaVarMap["HTTP_ACCEPT"] = client->getObjRequest().getHeaders().at("Accept");
    if (request.getHeaders().find("Connection") != request.getHeaders().end())
        this->_metaVarMap["HTTP_CONNECTION"] = client->getObjRequest().getHeaders().at("Connection");
    if (request.getHeaders().find("User-Agent") != request.getHeaders().end())
        this->_metaVarMap["HTTP_USER_AGENT"] = request.getHeaders().at("User-Agent");
    if (request.getHeaders().find("X-Secret-Header-For-Test") != request.getHeaders().end())
        this->_metaVarMap["HTTP_X_SECRET_HEADER_FOR_TEST"] = request.getHeaders().at("X-Secret-Header-For-Test");
    std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin();
    while (it != request.getHeaders().end()) {
    	if (this->_metaVarMap.find(Utils::replaceAll(Utils::toUppercase(it->first), '-', '_')) == this->_metaVarMap.end()) {
			this->_metaVarMap["HTTP_" + Utils::replaceAll(Utils::toUppercase(it->first), '-', '_')] = it->second;
		}
    	it ++;
    }
    this->_metaVarMap["REMOTE_IDENT"] = "login_user";
	this->_metaVarMap["REMOTE_USER"] = "user";

	//cette variable est necessaire pour lancer php-cgi
    this->_metaVarMap["REDIRECT_STATUS"] = "200";
	this->_var.metaVarArray = mapToArray(this->_metaVarMap);
}

/**
 * cette fonction convertit un vector en char**
 * @param vec : le vector a convertir
 * @return un array contenant les valeurs du vector
 */
char    **Cgi::vecToArray(std::vector<std::string> &vec)
{
    char    **array = new char*[vec.size() + 1];

    for (size_t i = 0; i < vec.size() ; i++) {
        array[i] = (char*)vec[i].c_str();
    }
    array[vec.size()] = NULL;
    return (array);
}

/**
 * cette fonction convertit une map en char**
 * @param map : la map a convertir
 * @return un array contenant les valeurs de la map
 */

char    **Cgi::mapToArray(std::map<std::string, std::string> &map)
{
    int												i = 0;
    std::string										tmp;
    char 											**env;

    if (!(env = (char**)malloc(sizeof(char*) * (map.size() + 1))))
        return NULL;
    for (std::map<std::string, std::string>::iterator it = this->_metaVarMap.begin(); it != this->_metaVarMap.end(); it++)
    {
        tmp = "" + it->first + "=" + it->second;
        env[i] = Utils::ft_strdup(tmp.c_str());
        i++;
    }
    env[i] = NULL;
    return (env);
}

/**
 * Cette fonction renvoie si la location contient un CGI, que la requête nécessite un appel vers un CGI
 * Et que le CGI est valide
 * @param request
 * @param location
 * @return oui si la location contient un CGI valide et que la requête appelle un CGI
 */
bool Cgi::isCGI (Request request, LocationConfig location)
{
	std::string cgiBin = location.getCgiBin(), cgiExtension = location.getCgiExtension();
	if (!cgiBin.empty() && !cgiExtension.empty()) {
		std::string path = request.getDefaultPath(location);
		size_t dotIndex = path.rfind('.');
		if (dotIndex != std::string::npos) {
			std::string extension = path.substr(dotIndex, path.size() - dotIndex);
			if (extension.compare(0, 4, ".bla") == 0 && request.getMethod() == "GET")
				return (false);
			/* on check si le cgibin existe */
			std::ifstream file(cgiBin.c_str(), std::ifstream::in);
			if (file.good() && file.is_open())
				return (extension.compare(0, cgiExtension.length(), cgiExtension) == 0);
		}
	}
	return (false);
}

const std::string &Cgi::getRequestFile() const {
    return _requestFile;
}

void Cgi::setRequestFile(const std::string &requestFile) {
    _requestFile = requestFile;
}
