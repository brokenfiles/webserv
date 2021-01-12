//
// Created by Timothe Lecouvreur on 11/01/2021.
//

#include "Cgi.hpp"

Cgi::Cgi()
{}

Cgi::~Cgi()
{

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

	setType(path.substr(nb + 1, path.length() - nb));
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
	(void)request;
	(*env)["SERVER_SOFTWARE"] = "Webserv/1.0";
	(*env)["SERVER_NAME"] = "localhost";
	(*env)["SERVER_PORT"] = Logger::to_string(PORT);
	(*env)["PATH_INFO"] = "";
	if (request.getMethod() == "GET")
		(*env)["QUERY_STRING"] = setQueryString(request.getQueryString());
	else
		(*env)["QUERY_STRING"] = "";
}

std::string		Cgi::setQueryString(std::string str)
{
	return (str);
}

int		Cgi::execute(Request request)
{

	int												pid;
	int												pipe_fd[2];
	char 											**env = convertEnv();
	char 											buffer[BUFFER];
	std::string										output = "";
	std::string										file = "cgi/bin/" + getType();
	char											**argv = convertArgv(request, file);

	pipe(pipe_fd);
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], 1) == -1)
			return (-1);
		if (execve(file.c_str(), argv, env) == -1)
			return (-1);
	}
	else
	{
		int 		ret;
		close(pipe_fd[1]);
		while ((ret = read(pipe_fd[0], &buffer, BUFFER - 1)) != 0)
		{
			buffer[ret] = 0;
			output += buffer;
		}
		setOutputBody(output);
	}
	freeAll(env, argv);
	return (0);
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

