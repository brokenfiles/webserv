/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: timlecou <timlecou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 10:07:31 by timlecou          #+#    #+#             */
/*   Updated: 2020/12/14 11:01:29 by timlecou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void	usage(char *str)
{
	std::cout << "usage: " << str << " <addr> <port>" << std::endl;
	exit(-1);
}

int		create_client(char *addr, int port)
{
	int		sock;
	struct	sockaddr_in	sin;

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
		std::cout << "Connect error" << std::endl;
	return (sock);
}

int		main(int ac, char **av)
{
	int						sock;
	int						cs;
	std::string 			str = "content-type: text/html;charset=UTF-8\nDate: Thu, 17 Dec 2020 14:27:36 GMT\nserver: timlecou\ncontent-encoding: gzip\n";
	int						r = 0;
	char					buff[1024];
	unsigned int			cslen;
	struct sockaddr_in		csin;

	if (ac != 3)
		usage(av[0]);
	sock = create_client(av[1], atoi(av[2]));
	cs = accept(sock, (struct sockaddr*)&csin, &cslen);
	write(sock, str.c_str(), str.length());
	close(sock);
	return (0);
}
