//
// Created by Timothe Lecouvreur on 11/01/2021.
//

#include "Cgi.hpp"
#include <sys/types.h>
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
void	Cgi::execute(Client *client, Response &response)
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

	t_execCGI   var;

	//on recupere argv et les variables d'environement
    var.argv = vecToArray(this->_argv);
    var.metaVarArray = mapToArray(this->_metaVarMap);

    pipe(var.pipe_fd);
    pipe(var.outfd);
    var.save_in = dup(STDIN_FILENO);
    var.save_out = dup(STDOUT_FILENO);
    dup2(var.outfd[1], 1);

	//on fork le processus en 2
    var.pid = fork();
    if (var.pid == 0)
	{
    	//processus fils
        close(var.outfd[1]);
        dup2(var.outfd[0], 0);
		dup2(var.pipe_fd[1], 1);

		//on execute le CGI
        execve(var.argv[0], var.argv, var.metaVarArray);
        close(var.outfd[0]);
	}
	else if (var.pid > 0)
	{
        //processus pere
		close(var.outfd[0]);

		//si c'est un requete POST on ecrit le body sur STDIN du processus fils
		if (client->getObjRequest().getMethod() == "POST")
			write(var.outfd[1], client->getObjRequest().getBody().c_str(), client->getObjRequest().getBody().length());

		close(var.outfd[1]);
		close(var.pipe_fd[1]);

        //on lit le retour du CGI et on le stock var.output
		while ((var.ret = read(var.pipe_fd[0], &var.buffer, BUFFER - 1)) != 0)
		{
            var.buffer[var.ret] = 0;
            var.output += var.buffer;
		}

		//on récupère le code de retour du CGI et des headers
		if (response.getLocation().getCgiExtension() != ".bla" || client->getObjRequest().getMethod() != "GET")
		{	//TODO récupérer le status code et les eventuels headers
			Parser parser;
			std::string tmp = var.output.substr(0, var.output.find("\r\n\r\n", 0));
			var.output.erase(0, var.output.find("\r\n\r\n", 0));
			Query query = parser.parseResponse(tmp + "\n");

			response.setCookies(query.getRawCookies());
			if (!query.getHeaders().empty()) {
				std::map<std::string, std::string>::const_iterator begin = query.getHeaders().begin();
				std::map<std::string, std::string> headers = response.getHeaders();
				if (query.getHeaders().find("Status") != query.getHeaders().end()) {
					response.setStatusCode(query.getHeaders().at("Status"));
					begin++;
				}
				for (; begin != query.getHeaders().end(); begin++) {
					headers[begin->first] = begin->second;
				}
				response.setHeaders(headers);
			}

		}
		//on remet STDIN et STDOUT sur leurs fd respectifs
		dup2(var.save_in, STDIN_FILENO);
		dup2(var.save_out, STDOUT_FILENO);

		//on remplace le body de la reponse par le retour du CGI
        response.setBody(var.output);

        //on free l'array de variables d'environement
        for (size_t i = 0; i < this->_metaVarMap.size(); ++i) {
            free(var.metaVarArray[i]);
        }
        free(var.metaVarArray);
        close(var.pipe_fd[0]);
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
}

/**
 * Cette fonction definie les META-VARIABLES requises par le cgi pour fonctionner,
 * elles sont stockées dans une map de la classe CGI
 * @param request : la requete faite par le client
 */
void Cgi::addMetaVariables(Response &response, Client *client)
{
	this->_metaVarMap["AUTH_TYPE"] = "NULL";
	this->_metaVarMap["REMOTE_ADDR"] = client->getIP();
	this->_metaVarMap["REQUEST_METHOD"] = client->getObjRequest().getMethod();
	this->_metaVarMap["CONTENT_LENGTH"] = Logger::to_string(client->getObjRequest().getBody().size());
    if (client->getObjRequest().getHeaders().find("Content-Type") != client->getObjRequest().getHeaders().end())
		this->_metaVarMap["CONTENT_TYPE"] = client->getObjRequest().getHeaders().find("Content-Type")->second.substr(0, client->getObjRequest().getHeaders().find("Content-Type")->second.length() - 1);
	this->_metaVarMap["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_metaVarMap["PATH_INFO"] = client->getObjRequest().getPath();
	this->_metaVarMap["PATH_TRANSLATED"] = getRequestFile();
	if (client->getObjRequest().getQueryString().empty())
	    this->_metaVarMap["QUERY_STRING"] = "";
	else
	    this->_metaVarMap["QUERY_STRING"] = client->getObjRequest().getQueryString();
	this->_metaVarMap["SCRIPT_FILENAME"] = getRequestFile();
	this->_metaVarMap["REQUEST_URI"] = client->getObjRequest().getPath();
	if (!client->getObjRequest().getQueryString().empty())
		this->_metaVarMap["REQUEST_URI"] += "?" + client->getObjRequest().getQueryString();
	this->_metaVarMap["SCRIPT_NAME"] = response.getLocation().getCgiBin();
	this->_metaVarMap["SERVER_NAME"] = client->getServerConfig().getServerName();
	this->_metaVarMap["SERVER_PORT"] = Logger::to_string(client->getServerConfig().getPort());
	this->_metaVarMap["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_metaVarMap["SERVER_SOFTWARE"] = "Webserv/1.0";
	if (client->getObjRequest().getHeaders().find("Cookie") != client->getObjRequest().getHeaders().end())
		this->_metaVarMap["HTTP_COOKIE"] = client->getObjRequest().getHeaders().at("Cookie");
    if (client->getObjRequest().getHeaders().find("Accept") != client->getObjRequest().getHeaders().end())
        this->_metaVarMap["HTTP_ACCEPT"] = client->getObjRequest().getHeaders().at("Accept");
    if (client->getObjRequest().getHeaders().find("Connection") != client->getObjRequest().getHeaders().end())
        this->_metaVarMap["HTTP_CONNECTION"] = client->getObjRequest().getHeaders().at("Connection");
    if (client->getObjRequest().getHeaders().find("User-Agent") != client->getObjRequest().getHeaders().end())
        this->_metaVarMap["HTTP_USER_AGENT"] = client->getObjRequest().getHeaders().at("User-Agent");
    this->_metaVarMap["REMOTE_IDENT"] = "login_user";
	this->_metaVarMap["REMOTE_USER"] = "user";

	//cette variable est necessaire pour lancer php-cgi
    this->_metaVarMap["REDIRECT_STATUS"] = "200";

    //TODO gérer l'initialisation de variables dans la conf
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
        env[i] = ft_strdup(tmp.c_str());
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
			/* on check si le cgibin existe */
			std::ifstream file(cgiBin.c_str(), std::ifstream::in);
			if (file.good() && file.is_open())
				return (cgiExtension == extension);
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

const std::string &Cgi::getCgiBin() const {
    return _cgiBin;
}

void Cgi::setCgiBin(const std::string &cgiBin) {
    _cgiBin = cgiBin;
}
