#include "Server.hpp"

//int errno_error(const char *str, int error)
//{
//	perror(str);
//	return (error);
//}

Server::Server()
{
	errno = 0;
	max_clients = MAX_CLIENT_HOST;
	memset(&client_socket, 0, sizeof(int) * max_clients);
}

Server::~Server()
{

}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &copy)
{
	if (this != &copy)
	{

	}
	return (*this);
}

int Server::setup()
{
	//création du socket (point de communication) PF_INET = Proto Internet IPv4, IPPROTO_TCP = TCP, et retourne un fd
	if ((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		return (logger.error("[SERVER]: socket: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

	//fonction pour rendre non bloquant un fd
	if (fcntl(server_sock, F_SETFL, O_NONBLOCK) < 0)
		return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

	logger.info("[SERVER]: TCP/IPv4 Socket ready to use!...", NO_PRINT_CLASS);

	//Set les vqleurs du socket server (s_addr = INADDR_ANY car server donc locqlhost + PORT)
	memset(&serv_socket_in, 0, sizeof(struct sockaddr_in));
	serv_socket_in.sin_family = AF_INET;
	serv_socket_in.sin_port = htons(PORT);
	serv_socket_in.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1") ou INADDR_ANY;

	//lie le point de communication (fd server) a l'addresse - port specifié dans setAddress();
	if (bind(server_sock, (const struct sockaddr *) &serv_socket_in, sizeof(serv_socket_in)) < 0)
		return (logger.error("[SERVER]: bind: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

	logger.info("[SERVER]: Binding success.", NO_PRINT_CLASS);

    //todo must be checked
    int opt = 1;
    if ((setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, 0)) == 0)
    {
        return (logger.error("[SERVER]: setsockopt: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
    }

	//met le socket en ecoute, attente de connexion.
	if (listen(server_sock, 20) < 0)
		return (logger.error("[SERVER]: listen" + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

	logger.info("[SERVER]: Waiting for incoming connexions...", NO_PRINT_CLASS);
	return (0);
}

int Server::setup_multiple_socket(std::list<Server*> &servers)
{
	std::list<Server*>::iterator it_serv = servers.begin();

	for (std::vector<ServerConfig>::iterator it = this->config.getServers().begin(); it != this->config.getServers().end(); it++)
	{
		std::cout << "OK" << std::endl;
		it_serv = servers.insert(it_serv, new Server());
//		(*it_serv)->setConfig()

		if (((*it_serv)->getServerSocket() = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)))
			return (logger.error("[SERVER]: socket: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

		if (fcntl((*it_serv)->getServerSocket(), F_SETFL, O_NONBLOCK) < 0)
			return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

		memset(&(*it_serv)->getServerAddr(), 0, sizeof(struct sockaddr_in));
		(*it_serv)->getServerAddr().sin_family = AF_INET;
		(*it_serv)->getServerAddr().sin_port = htons(PORT);
		(*it_serv)->getServerAddr().sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1") ou INADDR_ANY;
//		servers.insert()


	}
}



int Server::server_run(char **envp)
{
    int higher_fd, client_curr;
    int master_socket = Server::getSocketServer();
    fd_set fd_pool;

    while (1)
    {
        //return le fd le + haut pour select, et ajoute les fd à la pool;
        higher_fd = Server::setFD_MAX(fd_pool, master_socket);

        //ok quand un fd de la pool et ready to read/write;
        if (select(higher_fd + 1, &fd_pool, NULL, NULL, NULL) < 0)
            return (logger.error("[SERVER]: select: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

        //fd pret ? accept + stock le socket client
        if (FD_ISSET(master_socket, &fd_pool))
        {
            if (Server::accept_request(master_socket) == -1)
                return (-1);
            client_settled.push_back(new Client(Server::getSocketClient(), Server::getAddrClient()));
            logger.info(std::string("[SERVER]: Adding <") + Server::getClientIP() + ":" +
                        Logger::to_string(Server::getClientPort()) + std::string("> to pending sockets."),
                        NO_PRINT_CLASS);
        }

        //gère les requetes, receive + send
        for (std::list<Client *>::iterator it = client_settled.begin(); it != client_settled.end(); it++)
        {
            client_curr = (*it)->getSocket();
            if (FD_ISSET(client_curr, &fd_pool))
            {
                Response response;

                if (Server::read_request(client_curr) == -1)
                    return (-1);
                Client *toManage = (*it); //REQUETE DE CE CLIENT A GERE
                toManage->setRequest(Server::get_request());

//				std::cout << toManage->getRequest() << std::endl;
//
                response.prepareResponse(toManage->getRequest(), envp);
                if (Server::send_request(client_curr, response.stringify()) == -1)
                    return (-1);

//				std::cout << response.stringify() << std::endl;

                close(client_curr);
                logger.success(std::string("[SERVER]: Request successfully received/sent."), NO_PRINT_CLASS);
                logger.notice(std::string("[SERVER]: Disconnecting from  <") + Server::getClientIP() + ":" +
                              Logger::to_string(Server::getClientPort()) + std::string(">."), NO_PRINT_CLASS);
                it = client_settled.erase(it);
            }
        }
    }
    return (0);
}

int Server::setFD_MAX(fd_set &fd_pool, int master_socket)
{
    int higher_fd, client_curr;

    FD_ZERO(&fd_pool);
    FD_SET(master_socket, &fd_pool);
    higher_fd = master_socket;

    for (std::list<Client *>::iterator it = client_settled.begin(); it != client_settled.end(); it++)
    {
        client_curr = (*it)->getSocket();
        if (client_curr > 0)
            FD_SET(client_curr, &fd_pool);
        if (client_curr > higher_fd)
            higher_fd = client_curr;
    }
    return (higher_fd);
}

int Server::accept_request_core(int fd)
{
    int size = sizeof(client_socket_in);
    memset(&this->client_socket_in, 0, size);

    //récupére le fd_client, contenant header + body; stock info IP:PORT dans client_socket_in;
    if ((client_sock = accept(fd, (struct sockaddr *) &client_socket_in, (socklen_t *) &size)) < 0)
    {
        close(fd);
        return (logger.error("[SERVER]: accept: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
    }

    logger.success("[SERVER]: Connexion from <" + std::string(inet_ntoa(client_socket_in.sin_addr)) + ":" +
                   Logger::to_string(ntohs(client_socket_in.sin_port)) + ">.", NO_PRINT_CLASS);

    if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
        return (logger.error("[SERVER]: fcntl: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));

    return (0);
}

int Server::accept_request()
{
    return (accept_request_core(this->server_sock));
}

int Server::accept_request(int fd)
{
    return (accept_request_core(fd));
}

int Server::read_request_core(int fd)
{
    char buffer[BUFFER];
    int read = BUFFER - 1;
    std::string keeper("");

    while (read == (BUFFER - 1))
    {
        memset(buffer, 0, 100);
        read = recv(fd, buffer, BUFFER - 1, 0);
        if (read == -1)
            return (logger.error("[SERVER]: Could'nt read request: " + std::string(strerror(errno)), NO_PRINT_CLASS,
                                 -1));
        keeper += buffer;
    }
    this->request = keeper;

    return (0);
}

int Server::read_request()
{
    return (read_request_core(client_sock));
}

int Server::read_request(int fd)
{
    return (read_request_core(fd));
}

int Server::send_request_core(int fd, std::string toSend)
{
	if (send(fd, toSend.c_str(), toSend.length(), 0) != (int) toSend.length())
		return (logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS, -1));
	return (0);
}

int Server::send_request(std::string toSend)
{
	return (send_request_core(this->client_sock, toSend));
}

int Server::send_request(int fd, std::string toSend)
{
	return (send_request_core(fd, toSend));
}

std::string& Server::get_request()
{
	return (this->request);
}

int Server::getSocketServer()
{
	return (this->server_sock);
}

int Server::getSocketClient()
{
	return (this->client_sock);
}

struct sockaddr_in Server::getAddrServer()
{
	return (this->serv_socket_in);
}

struct sockaddr_in Server::getAddrClient()
{
	return (this->client_socket_in);
}

std::string Server::getClientIP(void)
{
	return (std::string(inet_ntoa(client_socket_in.sin_addr)));
}

int Server::getClientPort(void)
{
	return (ntohs(client_socket_in.sin_port));
}

void Server::setConfig(Config &conf)
{
	this->config = conf;
}

int &Server::getServerSocket(void)
{
	return (this->server_sock);
}

struct sockaddr_in &Server::getServerAddr()
{
	return (this->serv_socket_in);
}
