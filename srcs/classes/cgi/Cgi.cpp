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

    //on ajoute les META-VARIABLES
    addMetaVariables(client->getObjRequest(), response);

    //on ajoute le nom du binaire et celuidu fichier dans argv, qui sera passé au CGI dans un execve
    addArgv(response);

	int												pid;
	int												pipe_fd[2];
	int												outfd[2];
	int												save_in;
	int												save_out;
	char 											buffer[BUFFER];
	std::string										output = "";

	pipe(pipe_fd);
	pipe(outfd);
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	dup2(outfd[1], 1);
	pid = fork();
	if (pid == 0)
	{
		close(outfd[1]);
		dup2(outfd[0], 0);
		dup2(pipe_fd[1], 1);
		execve(getArgv()[0], getArgv(), getMetaVarArray());
	}
	else
	{
		int 		ret;

		close(outfd[0]);
		if (client->getObjRequest().getMethod() == "POST" && !response.getBody().empty())
			write(outfd[1], response.getBody().c_str(), response.getBody().length());
		close(outfd[1]);
		close(pipe_fd[1]);
		while ((ret = read(pipe_fd[0], &buffer, BUFFER - 1)) != 0)
		{
			buffer[ret] = 0;
			output += buffer;
		}
		if (response.getLocation().getCgiExtension() == "php")
			output.erase(0, 66);
		response.setBody(output);
		dup2(save_in, STDIN_FILENO);
		dup2(save_out, STDOUT_FILENO);
	}
}

/**
 * cette fonction rempli un array avec le nom du binaire de CGI a utiliser
 * et le path du fichier a executer
 * @param response : contient le nom du binaire dans une locationConfig
 */
void Cgi::addArgv(Response &response)
{
    //on crée un array et on le rempli avec les META-VARIABLES
    char** argv = new char*[3];

    for (int i = 0; i < 3; i++)
        argv[i] = new char;
    argv[0] = (char*)response.getLocation().getCgiBin().c_str();
    argv[1] = (char*)getRequestFile().c_str();
    argv[2] = NULL;
    setArgv(argv);
}

/**
 * Cette fonction definie les META-VARIABLES requise par le cgi pour fonctionner,
 * elles sont stockées dans une map de la classe CGI
 * @param request : la requete faite par le client
 */
void		Cgi::addMetaVariables(Request request, Response &response)
{
    std::map<std::string, std::string>	metaVarMap;

    metaVarMap["PATH_INFO"] = getRequestFile();
    metaVarMap["REQUEST_METHOD"] = request.getMethod();
    metaVarMap["SERVER_PROTOCOL"] = "HTTP/1.1";
    metaVarMap["REQUEST_URI"] = request.getPath();
    metaVarMap["QUERY_STRING"] = setQueryString(request.getQueryString());
    metaVarMap["REDIRECT_STATUS"] = "200";
    metaVarMap["SERVER_SOFTWARE"] = "Webserv/1.0";
    metaVarMap["SERVER_NAME"] = "localhost";
    metaVarMap["SERVER_PORT"] = Logger::to_string(PORT);
    metaVarMap["GATEWAY_INTERFACE"] = "CGI/1.1";
    metaVarMap["SCRIPT_FILENAME"] = getRequestFile();
    metaVarMap["REMOTE_ADDR"] = "127.0.0.1";
    metaVarMap["REMOTE_IDENT"] = "login_user";
    metaVarMap["REMOTE_USER"] = "user";
    //metaVarMap["HTTP_ACCEPT"] = request.getHeaders().at("Accept");
    //metaVarMap["HTTP_ACCEPT_LANGUAGE"] = request.getHeaders().at("Accept-Language");
    //metaVarMap["HTTP_USER_AGENT"] = request.getHeaders().at("User-Agent");
    metaVarMap["SCRIPT_NAME"] = request.getPath().substr(1, request.getPath().length() - 1);
    if (request.getHeaders().find("Content-Type") == request.getHeaders().end())
        metaVarMap["CONTENT_TYPE"] = "";
    else
        metaVarMap["CONTENT_TYPE"] = request.getHeaders().find("Content-Type")->second;
    if (request.getHeaders().find("Content-Length") == request.getHeaders().end())
        metaVarMap["CONTENT_TYPE"] = "0";
    else
        metaVarMap["CONTENT_LENGTH"] = Logger::to_string(response.getBody().length());
    if (request.getMethod() == "GET" && !metaVarMap["QUERY_STRING"].empty())
        metaVarMap["REQUEST_URI"] += "?" + request.getQueryString();


    //on crée un array et on le rempli avec les META-VARIABLES
    char** env = new char*[metaVarMap.size()];
    std::string         tmp;

    int i = 0;
    for (std::map<std::string, std::string>::iterator ite = metaVarMap.begin(); ite != metaVarMap.end(); ite++)
    {
        env[i] = new char;
        tmp = "";
        tmp.insert(0, ite->first);
        tmp.insert(tmp.length(), "=");
        tmp.insert(tmp.length(), ite->second);
        env[i] = (char*)tmp.c_str();
        i++;
    }
    env[i] = NULL;
    setMetaVarArray(env);
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

std::string		Cgi::setQueryString(std::string str) {
	return (str);
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

char **Cgi::getMetaVarArray() const {
    return _metaVarArray;
}

void Cgi::setMetaVarArray(char **metaVarArray) {
    _metaVarArray = metaVarArray;
}

char **Cgi::getArgv() const {
    return _argv;
}

void Cgi::setArgv(char **argv) {
    _argv = argv;
}

