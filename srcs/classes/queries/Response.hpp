//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "Query.hpp"
#include "Request.hpp"
#include "../server/Server.hpp"
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
	    void getHandler(Request request, int head);
		void putHandler(Request request);
		void deleteHandler(Request request);

		//formattage functions
		std::map<std::string, std::string>	basicHeaders(void);
		void 								fileExtension(std::map<std::string, std::string> *map, Request request);
		void								addBody(std::string path);

    public:
        Response();
        Response(Response &response);

        virtual ~Response();

        void setStatus(const std::string &status);
        const std::string &getStatus() const;

        void prepareResponse(std::string req);
        std::string stringify(void) const;

        std::string getCurrentTime(void);
};

std::ofstream&	operator<<(std::ofstream &o, const Response &res);

char    *ft_itoa(int nb);

#endif //WEBSERV_RESPONSE_HPP
