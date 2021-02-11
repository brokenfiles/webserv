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
#define META_VARIABLES_NB 17

class Client;
class Response;

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

        //le binaire à utiliser pour l'execution
        std::string                         _cgiBin;
public:
        Cgi();
		virtual ~Cgi();

		//execute function
		void		execute(Client *client, Response &response);

		//getters and setters
		const std::string                       &getRequestFile() const;
        void                                    setRequestFile(const std::string &requestFile);
        void                                    setCgiBin(const std::string &cgiBin);
        const std::string                       &getCgiBin() const;
        static bool                             isCGI(Request request, LocationConfig location);

		//meta var functions
        void addMetaVariables(Response &response, Client *pClient);
		void addArgv(Response &response);
		char **vecToArray(std::vector<std::string> &vec);
		char **mapToArray(std::map<std::string, std::string> &map);

};


//structure contenant toutes les variables necessaires à l'execution du CGI
typedef struct s_execCGI
{
    int			pid;
    int			pipe_fd[2];
    int			outfd[2];
    int			save_in;
    int			save_out;
    int         ret;
    char 		buffer[BUFFER];
    char        **argv;
    char        **metaVarArray;
    std::string	output;
}               t_execCGI;

char		*ft_strdup(const char *str);

#endif //WEBSERV_CGI_HPP
