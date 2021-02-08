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

class Cgi
{
private:
        std::map<std::string, std::string>  _metaVarMap;
        std::vector<std::string>            _argv;
        std::string                         _requestFile;
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
		void				        addMetaVariables(Request request, Response &response);
		std::string			        setQueryString(std::string path);

    void addArgv(Response &response);

    char **vecToArray(std::vector<std::string> &vec);

    char **mapToArray(std::map<std::string, std::string> map);
};

typedef struct s_execCGI
{
    int			pid;
    int			pipe_fd[2];
    int			outfd[2];
    int			save_in;
    int			save_out;
    int         ret;
    char 		buffer[BUFFER];
    std::string	output;
}               t_execCGI;

#endif //WEBSERV_CGI_HPP
