//
// Created by Timothe Lecouvreur on 11/01/2021.
//

#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../queries/Response.hpp"
#include "../queries/Request.hpp"
#include "../logger/Logger.hpp"
#define META_VARIABLES_NB 17

class Cgi
{
	private:
		std::map<std::string, std::string>	_env;
		std::string 						_type;
		std::string							_input_body;
		std::string							_output_body;
		unsigned int						_env_number;

	public:
		Cgi();
		virtual ~Cgi();

		//execute function
		int		execute(Request request);

		//getters and setters
		std::map<std::string, std::string>		getEnv() const;
		const std::string						&getType() const;
		void									setType(const std::string &type);
		void									setEnv(const std::map<std::string, std::string> &env);
		const std::string						&getInputBody() const;
		void									setInputBody(const std::string &inputBody);
		const std::string						&getOutputBody() const;
		void									setOutputBody(const std::string &outputBody);
		unsigned int							getEnvNumber() const;
		void									setEnvNumber(unsigned int envNumber);

		//utils functions
		void				parse(std::string body, std::string path, Request request, char **envp);
		void				freeAll(char **env, char **argv);
		char**				convertArgv(Request request, std::string file);

		//env var functions
		void				initEnv(char **envp, Request request);
		void				addMetaVariables(std::map<std::string, std::string> *env, Request request);
		std::string			setQueryString(std::string path);
		char**				convertEnv(void);

};

char		*ft_strdup(const char *str);

#endif //WEBSERV_CGI_HPP
