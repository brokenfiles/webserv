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
#include <sys/types.h>
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
	std::string 			headers = "content-type: text/html;charset=UTF-8\nDate: Thu, 17 Dec 2020 14:27:36 GMT\nserver: timlecou\ncontent-encoding: gzip\n";
	std::string 			body = "<html>\n"
								  "<head>\n"
								  "<title>Bonjour Tim</title>\n"
								  "</head>\n"
								  "<body>\n"
								  "<h1>Comment va tim</h1>\n"
								  "</body>\n"
								  "</html>";
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
		break ;
	}
	int ret = write(cs, body.c_str(), body.length());
	if (ret == 0)
		std::cout << "error" << std::endl;
	std::cout << "end of program" << std::endl;
	close(cs);
	close(sock);
	return (0);
}