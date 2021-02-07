#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include "Query.hpp"
#include "../client/Client.hpp"
#include "../config/LocationConfig.hpp"
#include "../config/ServerConfig.hpp"
#include "../cgi/Cgi.hpp"

class Client;

class Response : public Query
{
private:
	/** le code qui est retourné par response */
	std::string _statusCode;
	/** les messages des status code */
	std::map<int, std::pair<std::string, std::string> > _statusMessages;
	/** la correspondence entre extension et content type */
	std::map<std::string, std::string> _contentTypes;
	/** la location de la requête dans le serveur */
	LocationConfig _location;

	std::string getDirName(const std::string& file);

public:
	Response ();
	~Response ();
	Response (const Response &copy);
	Response &operator= (const Response &copy);

	std::string sendResponse (Client *client);
	LocationConfig &find_location (Client *client);
	std::string stringify () const;
	std::string currentDate ();
	std::string toLower (std::string string);
	void getHandler (Client *client);
	void setDefaultHeaders (Client *client, ServerConfig &server);
	bool isMethodValid (const std::string &method);
	void setContentType (Client *client);
	void setDefaultStatusCodes ();
	void setDefaultExtensions ();

	/*
	 * Getters
	 */
	const std::string &getStatusCode () const;
	const std::map<int, std::pair<std::string, std::string> > &getStatusMessages () const;
	std::string getMessageCode (int code);
	std::string getPathWithSlash (std::string path);
	const std::map<std::string, std::string> &getContentTypes () const;
	const LocationConfig &getLocation () const;
	const std::string &getFileCode (int code);

	/**
	 * Setters
	 */
	void setMessageCode (int code, const std::string& message);
	void setFileCode (int code, const std::string& file);
	void addError (int code, const std::string &message, const std::string& file);
	/*
	 * Exceptions
	 */
	class NoLocationException : public std::exception
	{
		virtual const char *what () const throw();
	};
};

#endif
