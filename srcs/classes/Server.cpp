#include "Server.hpp"

int     errno_error(const char *str, int error)
{
    perror(str);
    return (error);
}


Server::Server()
{
    errno = 0;
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
    //création du socket TCP IPv4 - retourne un file descriptor;
    if ((server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return (errno_error("[SERVER]: socket", -1));

    std::cout << "[SERVER]: Socket ready to use!...\n";

    Server::setAddress();

    //affectation d'un nom au socket "sock_fd", affecte <adresse> <port> specifié dans setAddress();
    if (bind(server_sock, (const struct sockaddr *)&serv_socket_in, sizeof(serv_socket_in)) < 0)
        return (errno_error("[SERVER]: bind", -1));

    std::cout << "[SERVER]: Binding succed!...\n";

    //ecoute sur le socket server, queue de 20 request;
    if (listen(server_sock, 20) < 0)
        return (errno_error("[SERVER]: listen", -1));

    std::cout << "[SERVER]: Waiting for incoming connexions...\n";
    return (0);
}

void Server::setAddress()
{
    //initialisation des member de l'objet server;
    memset(&serv_socket_in, 0, sizeof(struct sockaddr_in));
    serv_socket_in.sin_family = AF_INET;
    serv_socket_in.sin_port = htons(PORT);
    serv_socket_in.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr("127.0.0.1") ou INADDR_ANY;
}

int Server::accept_request()
{
    int size;

    //récupére le fd_client, contenant header + body; stock info IP:PORT dans client_socket_in;
    if ((client_sock = accept(server_sock, (struct sockaddr *) &client_socket_in, (socklen_t *) &size)) < 0)
    {
        close (server_sock);
        return (errno_error("[SERVER]: accept", 1));
    }
    std::cout << "[SERVER]: Connexion from < " << inet_ntoa(client_socket_in.sin_addr) << ":" << ntohs(client_socket_in.sin_port) << " > accepted.\n";
    return (0);
}

int Server::read_request()
{
    char buffer[BUFFER];
    int read = BUFFER -1;
    std::string keeper("");

    while (read == (BUFFER - 1))
    {
        memset(buffer, 0, 100);
        read = recv(client_sock, buffer, BUFFER - 1, 0);
        if (read == -1)
            return (errno_error("[SERVER]: Could'nt read request", -1));
        keeper += buffer;
    }
    this->request = keeper;
    return (0);
}

int Server::send_request()
{
    std::string toSend;
//    std::ifstream file("srcs/home/index.html", std::ios::in);
//    if (!file.is_open())
//        std::cout << "ntm" << std::endl;
//
//    std::getline(file, toSend, '\0');

    std::string str("mes couilles dans du buerre couvertes de jambon et de gruyères gratiné");

    if (send(client_sock, str.c_str(), str.length(), 0) < 0)
        return (errno_error("[SERVER]: send", -1));
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