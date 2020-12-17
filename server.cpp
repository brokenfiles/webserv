/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timlecou <timlecou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 10:07:31 by timlecou          #+#    #+#             */
/*   Updated: 2020/12/14 10:55:10 by timlecou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void	usage(char *str)
{
	std::cout << "usage: " << str << " <port> <query file>" << std::endl;
	exit(-1);
}

int		create_server(int port)
{
	int		sock;
	struct	sockaddr_in	sin;

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
		std::cout << "Bind error\n";
	listen(sock, 42);
	return (sock);
}

int		main(int ac, char **av)
{
	int						sock;
	int						cs;
	int						r = 0;


	std::string newStr;
	std::ifstream ifs(av[2], std::ios::in);

	if (ifs.is_open())
		std::getline(ifs, newStr, '\0');
	else {
		std::cerr << "cannot open the file" << std::endl;
		return (1);
	}
	ifs.close();


	char					buff[1024];
	unsigned int			cslen;
	struct sockaddr_in		csin;

	if (ac != 3)
		usage(av[0]);
	sock = create_server(atoi(av[1]));
	cs = accept(sock, (struct sockaddr*)&csin, &cslen);
	while ((r = read(cs, buff, 1023)) > 0)
	{
		buff[r] = '\0';
		std::cout << buff;
		break ;
	}
	int ret = write(cs, newStr.c_str(), newStr.length());
	if (ret == 0)
		std::cout << "error" << std::endl;
	std::cout << "end of program" << std::endl;
	close(cs);
	close(sock);
	return (0);
}