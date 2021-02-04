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
	    std::string                         _path;
	    std::string                         _index;
	    std::string                         _complete_path;
        std::map<std::string, std::string>  _location;

public:
    const std::string &getCompletePath() const;

    void setCompletePath(const std::string &completePath);

private:

    //method handlers
	    void getHandler(Request request, int head, char **envp, ServerConfig server);
		void putHandler(Request request, char **envp, ServerConfig server);
		void deleteHandler(Request request, char **envp, ServerConfig server);
        std::string							execute_cgi(std::string, std::string, Request, char **envp);

    //formattage functions
		std::map<std::string, std::string>	basicHeaders(void);
		void 								fileExtension(std::map<std::string, std::string> *map, Request request);
		void								addBody(Request request, char **envp);
        void                                setErrorStatus(int status_code);
        void                                init_location(ServerConfig server, Request request);
		//utils functions
		bool								isCGI(std::string);
        void                                fileExist(std::string file, std::map<std::string, std::string> map, Request request);


public:
    const std::string &getPath() const;

    void setPath(const std::string &path);

    const std::string &getIndex() const;

    void setIndex(const std::string &index);

    const std::map<std::string, std::string> &getLocation() const;

    void setLocation(const std::map<std::string, std::string> &location);

    public:
        Response();
        Response(Response &response);

        virtual ~Response();

        void setStatus(const std::string &status);
        const std::string &getStatus() const;

        void prepareResponse(std::string req, char **envp, Config config);
        std::string stringify(void) const;

        std::string getCurrentTime(void);
        std::map<std::string, std::string> find_location(ServerConfig server);
};

std::ofstream&	operator<<(std::ofstream &o, const Response &res);

#endif //WEBSERV_RESPONSE_HPP
