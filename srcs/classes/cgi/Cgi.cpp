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

void	Cgi::execute(Client *client, Response &response)
{
	(void)client;
	(void)response;
//	response.setBody("blablabla");
/*
	int												pid;
	int												pipe_fd[2];
	int												outfd[2];
	int												save_in;
	int												save_out;
	char 											**env = convertEnv();
	char 											buffer[BUFFER];
	std::string										output = "";
	std::string										file = "cgi/bin/cgi_tester";// + getType();
	char											**argv = convertArgv(request, file);

	pipe(pipe_fd);
	pipe(outfd);
	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (dup2(outfd[1], 1) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		close(outfd[1]);
		if (dup2(outfd[0], 0) == -1)
			return (-1);
		if (dup2(pipe_fd[1], 1) == -1)
			return (-1);
		if (execve(file.c_str(), argv, env) == -1)
			return (-1);
	}
	else
	{
		int 		ret;
		int 		status;

		close(outfd[0]);
		if (request.getMethod() == "POST" && !getInputBody().empty())
			write(outfd[1], getInputBody().c_str(), getInputBody().length());
		close(outfd[1]);
		close(pipe_fd[1]);
		waitpid(-1, &status, 0);
		while ((ret = read(pipe_fd[0], &buffer, BUFFER - 1)) != 0)
		{
			buffer[ret] = 0;
			output += buffer;
		}
		if (getType() == "php")
			output.erase(0, 66);
		setOutputBody(output);
		dup2(save_in, STDIN_FILENO);
		dup2(save_out, STDOUT_FILENO);
	}
	freeAll(env, argv);
	return (0);
 */
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

void Cgi::setEnv(const std::map<std::string, std::string> &env)
{
	_env = env;
}

std::map<std::string, std::string> Cgi::getEnv() const
{
	return _env;
}

const std::string &Cgi::getType() const
{
	return _type;
}

void Cgi::setType(const std::string &type)
{
	_type = type;
}

const std::string &Cgi::getInputBody() const
{
	return _input_body;
}

void Cgi::setInputBody(const std::string &inputBody)
{
	_input_body = inputBody;
}

const std::string &Cgi::getOutputBody() const
{
	return _output_body;
}

void Cgi::setOutputBody(const std::string &outputBody)
{
	_output_body = outputBody;
}

unsigned int Cgi::getEnvNumber() const
{
	return _env_number;
}

void Cgi::setEnvNumber(unsigned int envNumber)
{
	_env_number = envNumber;
}

void 		Cgi::parse(std::string body, std::string path, Request request, char **envp)
{
	int		nb = path.rfind('.', path.length());

	if (path.substr(nb + 1, path.length() - nb) == "php")
		setType("php");
	else if (path.substr(nb + 1, path.length() - nb) == "rb")
		setType("ruby");
	else if (path.substr(nb + 1, path.length() - nb) == "bla")
		setType("bla");
	setInputBody(body);
	initEnv(envp, request);
}

void		Cgi::initEnv(char **envp, Request request)
{
	std::map<std::string, std::string>	env;
	std::string 						tmp;
	int 								nb = 0;

	for (int i = 0; envp[i]; i++)
	{
		tmp = Logger::to_string(envp[i]);
		nb = tmp.find("=", 0);
		env[tmp.substr(0, nb)] = tmp.substr(nb + 1, tmp.length());
	}
	addMetaVariables(&env, request);
	setEnvNumber(env.size());
	setEnv(env);
}

void		Cgi::addMetaVariables(std::map<std::string, std::string> *env, Request request)
{
	(*env)["PATH_INFO"] = request.getPath();
	(*env)["REQUEST_METHOD"] = request.getMethod();
	(*env)["SERVER_PROTOCOL"] = "HTTP/1.1";
	(*env)["REQUEST_URI"] = request.getPath();
	(*env)["QUERY_STRING"] = setQueryString(request.getQueryString());
	(*env)["REDIRECT_STATUS"] = "200";
	(*env)["SERVER_SOFTWARE"] = "Webserv/1.0";
	(*env)["SERVER_NAME"] = "localhost";
	(*env)["SERVER_PORT"] = Logger::to_string(PORT);
	(*env)["GATEWAY_INTERFACE"] = "CGI/1.1";
	(*env)["SCRIPT_NAME"] = request.getPath().substr(1, request.getPath().length() - 1);
	if (request.getHeaders().find("Content-Type") == request.getHeaders().end())
		(*env)["CONTENT_TYPE"] = "";
	else
		(*env)["CONTENT_TYPE"] = request.getHeaders().find("Content-Type")->second;
	if (request.getHeaders().find("Content-Length") == request.getHeaders().end())
		(*env)["CONTENT_TYPE"] = "0";
	else
		(*env)["CONTENT_LENGTH"] = Logger::to_string(getInputBody().length());
	(*env)["SCRIPT_FILENAME"] = HOME + request.getPath();
	(*env)["REMOTE_ADDR"] = "127.0.0.1";
	(*env)["REMOTE_IDENT"] = "login_user";
	(*env)["REMOTE_USER"] = "user";
	if (request.getMethod() == "GET" && !(*env)["QUERY_STRING"].empty())
		(*env)["REQUEST_URI"] += "?" + request.getQueryString();
	//(*env)["HTTP_ACCEPT"] = request.getHeaders().at("Accept");
	//(*env)["HTTP_ACCEPT_LANGUAGE"] = request.getHeaders().at("Accept-Language");
	//(*env)["HTTP_USER_AGENT"] = request.getHeaders().at("User-Agent");
}

std::string		Cgi::setQueryString(std::string str)
{
	return (str);
}

void	Cgi::print_env(void)
{
	std::map<std::string, std::string>::iterator 	ite;

	for (ite = getEnv().begin(); ite != getEnv().end(); ite++)
		std::cout << ite->first << "=" << ite->second <<std::endl;
}

char**		Cgi::convertEnv(void)
{
	int												i = 0;
	std::map<std::string, std::string>::iterator	ite;
	std::map<std::string, std::string>				envp = getEnv();
	std::string										tmp;
	char 											**env;

	if (!(env = (char**)malloc(sizeof(char*) * (getEnvNumber() + 1))))
		return NULL;
	for (ite = envp.begin(); ite != envp.end(); ite++)
	{
		tmp = "";
		tmp.insert(0, ite->first);
		tmp.insert(tmp.length(), "=");
		tmp.insert(tmp.length(), ite->second);
		env[i] = ft_strdup(tmp.c_str());
		i++;
	}
	env[i] = NULL;
	return (env);
}

char**	Cgi::convertArgv(Request request, std::string file)
{
	char											**argv;

	if (!(argv = (char**)malloc(sizeof(char*) * 3)))
		return NULL;
	argv[0] = ft_strdup(file.c_str());
	argv[1] = ft_strdup((HOME + request.getPath()).c_str());
	argv[2] = NULL;
	return (argv);
}

void		Cgi::freeAll(char **env, char **argv)
{
	int		i = 0;

	for (int j = 0; j < i; j++)
		free(env[j]);
	free(env);
	free(argv[0]);
	free(argv);
}

