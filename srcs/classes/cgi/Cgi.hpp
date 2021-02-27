//
// Created by Timothe Lecouvreur on 11/01/2021.
//

#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../queries/Response.hpp"
#include "../queries/Request.hpp"
#include "../logger/Logger.hpp"
#include "../client/Client.hpp"

class Client;
class Response;
class Request;

//structure contenant toutes les variables necessaires à l'execution du CGI
typedef struct s_execCGI
{
	int			pid;
	int			input_fd;
	int			output_fd;
	int			save_in;
	int			save_out;
	int         ret;
	int 		status;
	char 		buffer[BUFFER];
	char        **argv;
	char        **metaVarArray;
	std::string	output;
}               t_execCGI;


/**
 * Cette classe gère l'execution des programes CGI
 */
class Cgi
{
private:

		//une map contenant les variables d'environement passées au CGI lors de l'execution
        std::map<std::string, std::string>  _metaVarMap;

		//un vector contenant le binaire à utiliser et le fichier à executer
        std::vector<std::string>            _argv;

        //le fichier à executer
        std::string                         _requestFile;

        //les variables necessaire pour le execve
        t_execCGI							_var;

        Client								*_client;

        std::string							_requestBody;
public:
        Cgi();
		virtual ~Cgi();

		void		launch(Client *client, Response &response);
		void		execute(Response &response);

		//getters and setters
		const std::string                       &getRequestFile() const;
        void                                    setRequestFile(const std::string &requestFile);
        static bool                             isCGI(Request request, LocationConfig location);

		//meta var functions
        void addMetaVariables(Response &response, Client *Client);
		void addArgv(Response &response);
		char **vecToArray(std::vector<std::string> &vec);
		char **mapToArray(std::map<std::string, std::string> &map);

		void	getCGIReturn(Response &response);

};

#endif //WEBSERV_CGI_HPP
