//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "Query.hpp"
#include "Request.hpp"
#include "../server/Server.hpp"
#include "../config/Config.hpp"
#include "../cgi/Cgi.hpp"
#define CGI "php;cgi"
#ifndef MAKE
#define HOME "srcs/home"
#endif
#ifdef MAKE
#define HOME "srcs/home"
#endif

class Response : public Query
{
    private:
	    std::string 						_status;

	    //method handlers
	    void getHandler(Request request, int head, char **envp, ServerConfig server);
		void putHandler(Request request, char **envp, ServerConfig server);
		void deleteHandler(Request request, char **envp, ServerConfig server);

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

        void prepareResponse(std::string req, char **envp, Config config);
        std::string stringify(void) const;

        std::string getCurrentTime(void);
        std::map<std::string, std::string> find_location(ServerConfig server, std::string path);
};

std::ofstream&	operator<<(std::ofstream &o, const Response &res);

#endif //WEBSERV_RESPONSE_HPP
