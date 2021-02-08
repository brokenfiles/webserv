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
        char                                **_metaVarArray;
        char                                **_argv;
        std::string                         _requestFile;
        std::string                         _cgiBin;
public:
        Cgi();
		virtual ~Cgi();

		//execute function
		void		execute(Client *client, Response &response);

		//getters and setters
		const std::string                       &getRequestFile() const;
        char                                    **getArgv() const;
        void                                    setArgv(char **argv);
        void                                    setRequestFile(const std::string &requestFile);
        void                                    setCgiBin(const std::string &cgiBin);
        const std::string                       &getCgiBin() const;
        char                                    **getMetaVarArray() const;
        void                                    setMetaVarArray(char **metaVarArray);
        static bool                             isCGI(Request request, LocationConfig location);

		//meta var functions
		void				        addMetaVariables(Request request, Response &response);
		std::string			        setQueryString(std::string path);

    void addArgv(Response &response);
};

char		*ft_strdup(const char *str);

#endif //WEBSERV_CGI_HPP
