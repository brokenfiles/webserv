//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "Query.hpp"
#include "Request.hpp"
#include "../server/Server.hpp"

class Response : public Query
{
    private:
	    std::string 						_status;

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


#endif //WEBSERV_RESPONSE_HPP
