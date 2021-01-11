//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "Query.hpp"
#include "Request.hpp"
#include "../server/Server.hpp"
#include "../cgi/Cgi.hpp"
#define CGI "php;cgi"
#ifndef MAKE
#define HOME "home"
#endif
#ifdef MAKE
#define HOME "srcs/home"
#endif

class Response : public Query
{
    private:
	    std::string 						_status;

	    //method handlers
	    void getHandler(Request request, int head, char **envp);
		void putHandler(Request request, char **envp);
		void deleteHandler(Request request, char **envp);

		//formattage functions
		std::map<std::string, std::string>	basicHeaders(void);
		void 								fileExtension(std::map<std::string, std::string> *map, Request request);
		void								addBody(std::string path, Request request, char **envp);

		//utils functions
		bool								isCGI(std::string);

		std::string							execute_cgi(std::string, std::string, Request, char **envp);
    public:
        Response();
        Response(Response &response);

        virtual ~Response();

        void setStatus(const std::string &status);
        const std::string &getStatus() const;

        void prepareResponse(std::string req, char **envp);
        std::string stringify(void) const;

        std::string getCurrentTime(void);
};

std::ofstream&	operator<<(std::ofstream &o, const Response &res);

#endif //WEBSERV_RESPONSE_HPP
