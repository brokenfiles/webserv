//
// Created by Timothe Lecouvreur on 05/01/2021.
//

#ifndef WEBSERV_RESPONSE_HPP
#define WEBSERV_RESPONSE_HPP
#include "../queries/Query.hpp"
#include "../queries/Request.hpp"
#include "../server/Server.hpp"

class Response : public Query
{
private:
	std::string 						_status;

public:

	Response(Response &response) : Query(response)
	{}

	Response(void)
	{}

	virtual ~Response()
	{}

	void setStatus(const std::string &status) {
		_status = status;
	}

	const std::string &getStatus() const {
		return _status;
	}

	//assemble les elements de la requete dans une string et la renvoi
	std::string 	stringify(void) const
	{
		std::string		ret;
		std::map<std::string, std::string>::const_iterator	ite;

		ret.insert(ret.length(), "HTTP/1.1 ");
		ret.insert(ret.length(), getStatus());
		ret.push_back('\r');
		ret.push_back('\n');
		for(ite = getHeaders().begin(); ite != getHeaders().end(); ite++)
		{
			ret.insert(ret.length(), ite->first);
			ret.push_back(':');
			ret.push_back(' ');
			ret.insert(ret.length(), ite->second);
			ret.push_back('\n');
		}
		ret.push_back('\r');
		ret.push_back('\n');
		ret.insert(ret.length(), getBody());
		return (ret);
	}

	void	prepareResponse(std::string req)
	{
		std::map<std::string, std::string> map;	//contiendra les headers
		std::string newStr;
		std::string path = "home";	//definie dans quel repertoire se trouve le site

		Request request;
		Parser parser;

		request = parser.parse(req);

		if (request.getPath() == "/")	//chemin par defaut si rien n'est spécifié
		{
			path.insert(path.length(), "/index.html");
			setStatus("200 OK");
		}
		else	//si on a un path de spécifié
		{
			path.insert(path.length(), request.getPath());	//on ajoute le path spécifié a home on obtient home/PATH
			std::ifstream ifs(path, std::ios::in);
			if (ifs.good())	//vérifie si le fichier existe
			{//si le fichier demandé existe
				if (ifs.is_open())
				{	//on vérifie l'extension du fichier et ajuste le header Content-Type en fonction
					if (request.getPath().compare(request.getPath().length() - 5, 5, ".html") == 0)
						map["Content-Type"] = "text/html";
					else if (request.getPath().compare(request.getPath().length() - 4, 4, ".css") == 0)
						map["Content-Type"] = "text/css";
					else if (request.getPath().compare(request.getPath().length() - 3, 3, ".js") == 0)
						map["Content-Type"] = "application/javascript";
					setStatus("200 OK");
				}
				else
					std::cout << "can't open file" << std::endl;
			}
			else	//si le fichier n'existe pas
			{
				setStatus("404 Not Found");	//on met le status 404 Not Found
				map["Content-Type"] = "text/html";
				path = "error/NotFound.html";	//on redirige le path sur le fichier d'erreur 404 se trouvant dans error/NotFound.html
			}
			ifs.close();
		}

		std::ifstream file(path, std::ios::in);
		std::getline(file, newStr, '\0');	//récupération du body dans newStr
		setBody(newStr);	//ajout du body
		map["Server"] = "Webserv";
		map["Date"] = getCurrentTime();
		setHeaders(map);
		file.close();
	}

	//fonction qui donne la date au format HTTP dans une string
	std::string getCurrentTime(void)
	{
		struct timeval tv;
		std::string ret;
		char str[64];
		time_t time;
		struct tm *current = NULL;

		gettimeofday(&tv, NULL);
		time = tv.tv_sec;
		current = localtime(&time);	//localtime est interdite faudra la recoder a l'occas
		strftime(str, sizeof str, "%a, %d %b %G %T GMT", current);
		ret.assign(str);
		return (ret);
	}
};


#endif //WEBSERV_RESPONSE_HPP
