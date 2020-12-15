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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	usage(char *str)
{
	std::cout << "usage: " << str << " <port>" << std::endl;
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
	char					buff[1024];
	unsigned int			cslen;
	struct sockaddr_in		csin;

	if (ac != 2)
		usage(av[0]);
	sock = create_server(atoi(av[1]));
	cs = accept(sock, (struct sockaddr*)&csin, &cslen);
	while ((r = read(cs, buff, 1023)) > 0)
	{
		buff[r] = '\0';
		std::cout << buff;
	}
	close(cs);
	close(sock);
	return (0);
}
