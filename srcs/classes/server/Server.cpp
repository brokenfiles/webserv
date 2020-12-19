#include "Server.hpp"

//int errno_error(const char *str, int error)
//{
//	perror(str);
//	return (error);
//}

Server::Server()
{
	errno = 0;
	max_clients = MAX_CLIENT_HOST,
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
    int opt = 1;

	//création du socket TCP IPv4 - retourne un file descriptor;
	if ((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
	    logger.error("[SERVER]: socket: " + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }

	logger.info("[SERVER]: Socket ready to use!...", NO_PRINT_CLASS);

	//allow multiple connection, work without this, but idk
	if ((setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 0))  == 0)
    {
        logger.error("[SERVER]: setsockopt: " + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }

	Server::setAddress();

	//affectation d'un nom au socket "sock_fd", affecte <adresse> <port> specifié dans setAddress();
	if (bind(server_sock, (const struct sockaddr *) &serv_socket_in, sizeof(serv_socket_in)) < 0)
    {
        logger.error("[SERVER]: bind: " + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }

	logger.info("[SERVER]: Binding success", NO_PRINT_CLASS);

	//ecoute sur le socket server, queue de 20 request;
	if (listen(server_sock, 20) < 0)
    {
        logger.error("[SERVER]: listen" + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }

//	logger.info("[SERVER]: Accepting connextions on port" + , NO_PRINT_CLASS);
	logger.info("[SERVER]: Waiting for incoming connexions...", NO_PRINT_CLASS);
	return (0);
}

void Server::setAddress()
{
	//initialisation des member de l'objet server;
	memset(&serv_socket_in, 0, sizeof(struct sockaddr_in));
	serv_socket_in.sin_family      = AF_INET;
	serv_socket_in.sin_port        = htons(PORT);
	serv_socket_in.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1") ou INADDR_ANY;
}

int Server::accept_request_core(int fd)
{
    int size = sizeof(client_socket_in);

    //récupére le fd_client, contenant header + body; stock info IP:PORT dans client_socket_in;
    if ((client_sock = accept(fd, (struct sockaddr *) &client_socket_in, (socklen_t *) &size)) < 0)
    {
        close(fd);
        logger.error("[SERVER]: accept: " + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }
    logger.info("[SERVER]: Connexion from <" + std::string(inet_ntoa(client_socket_in.sin_addr)) + ":" +
                std::to_string(ntohs(client_socket_in.sin_port)) + "> accepted.", NO_PRINT_CLASS);
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
    char        buffer[BUFFER];
    int         read = BUFFER - 1;
    std::string keeper("");

    while (read == (BUFFER - 1))
    {
        memset(buffer, 0, 100);
        read = recv(fd, buffer, BUFFER - 1, 0);
        if (read == -1)
        {
            logger.error("[SERVER]: Could'nt read request: " + std::string(strerror(errno)), NO_PRINT_CLASS);
            return (-1);
        }
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

int Server::send_request(std::string toSend)
{
	if (send(client_sock, toSend.c_str(), toSend.length(), 0) != (int)toSend.length())
    {
        logger.error("[SERVER]: send: " + std::string(strerror(errno)), NO_PRINT_CLASS);
        return (-1);
    }
	return (0);
}

int Server::server_run()
{
    int higher_fd, client_curr, i = 0;;
    int master_socket = Server::getSocketServer();
    fd_set fd_pool;

    while (1)
    {
        FD_ZERO(&fd_pool);
        FD_SET(master_socket, &fd_pool);
        higher_fd = master_socket;

        for (i = 0; i < max_clients; i++)
        {
            client_curr = client_socket[i];
            if (client_curr > 0)
                FD_SET(client_curr, &fd_pool);
            if (client_curr > higher_fd)
                higher_fd = client_curr;
        }

        if (select(higher_fd + 1, &fd_pool, NULL, NULL, NULL) < 0)
        {
            logger.error("[SERVER]: select: " + std::string(strerror(errno)), NO_PRINT_CLASS);
            return (-1);
        }

        if (FD_ISSET(master_socket, &fd_pool))
        {
            if (Server::accept_request(master_socket) == -1)
                return (-1);
            if (Server::send_request("Welcome!") == -1)
                return (-1);
            for (i = 0; i < max_clients; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = Server::getSocketClient();
                    logger.success("[SERVER]: Adding to list of sockets : " + std::to_string(i), NO_PRINT_CLASS);
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            client_curr = client_socket[i];
            if (FD_ISSET(client_curr, &fd_pool))
            {
                //need optimisation selon type de connexion;
                Server::read_request(client_curr);
                close(client_curr);
                client_socket[i] = 0;
                logger.info("[SERVER]: Host disconnected. Socket: " + std::to_string(i), NO_PRINT_CLASS);
            }
        }
    }
    return (0);
}

int Server::close_sock()
{
	close(this->server_sock);
	close(this->client_sock);

	return (0);
}

std::string Server::get_request()
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