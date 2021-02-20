#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP

#include <iostream>
#include "Query.hpp"
#include "../client/Client.hpp"
#include "../config/LocationConfig.hpp"
#include "../config/ServerConfig.hpp"
#include "../cgi/Cgi.hpp"

class Response : public Query
{
private:
	/** le code qui est retourné par response */
	std::string _statusCode;
	/** les messages des status code */
	std::map<int, std::pair<std::string, std::string> > _statusMessages;
	/** la correspondence entre extension et content type */
	std::map<std::string, std::string> _contentTypes;
	/** l'extension du langage s'il y en a un spéficié */
	std::string _extLanguage;
	/** la location de la requête dans le serveur */
	LocationConfig _location;

	std::string getDirName(const std::string& file);
	void removeDir (const std::string &path, Client *client);

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
	void setDefaultHeaders (Client *client, ServerConfig &server);
	bool isMethodValid (const std::string &method);
	void setContentType (Client *client);
	void setDefaultStatusCodes ();
	void setDefaultExtensions ();
	void replace(std::string &fileContent, std::string replace, std::string newString);

	/*
	 * Methods Handlers
	 */
	void getHandler (Client *client);
	void putHandler (Client *client);
	void postHandler (Client *client);
	void deleteHandler (Client *client);
	void handleAcceptLanguage (Client *client, const std::string &requestFile);
	void handleAcceptCharset (Client *client);

	/*
	 * Getters
	 */
	const std::string &getStatusCode () const;
	std::map<int, std::pair<std::string, std::string> > &getStatusMessages ();
	std::string getMessageCode (int code);
	std::string getPathWithSlash (std::string path);
	std::map<std::string, std::string> &getContentTypes ();
	LocationConfig &getLocation ();
	const std::string &getFileCode (int code);
	std::string getFilesInDirectory (const std::string &path, Client *client);
	std::string getLastModified (const std::string &file);

	/**
	 * Setters
	 */
	void setMessageCode (int code, const std::string& message);
	void setFileCode (int code, const std::string& file);
	void addError (int code, const std::string &message, const std::string& file);
	void setStatusCode(const std::string &statusCode);
	/*
	 * Exceptions
	 */
	class NoLocationException : public std::exception
	{
		virtual const char *what () const throw();
	};
	class CantOpenDirectoryException : public std::exception
	{
		virtual const char *what () const throw();
	};
	void displayErrors ();
	void tryDirectoryListing (const std::string &path, Client *client);
	bool authenticate (Client *client);
};

#endif
