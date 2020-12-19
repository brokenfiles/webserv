fd_set fds, readfds;
int i = 0, rc;
int numsocks = 0, maxsocks = 10;
int clientsock[10];

FD_ZERO(&fds);
FD_SET(server.getSocketServer(), &fds);
while (1)
{
readfds = fds;

std::cout << "la\n";


rc = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
if (rc == -1)
{
std::cout << "wow\n";
break;
}
std::cout << "ici\n";

for (i = 0; i < FD_SETSIZE; i++)
{
//            std::cout << "gffgsfdsfdsffg\n";
if (FD_ISSET(i, &readfds))
{
std::cout << "gffgsfdsfdsffg\n";

if (i == server.getSocketServer())
{
//                    std::cout << "gffgsfg\n";

if (numsocks < maxsocks)
{
std::cout << "ici2\n";

server.accept_request();
clientsock[numsocks] = server.getSocketClient();
std::cout << "la2\n";

FD_SET(clientsock[numsocks], &fds);
numsocks++;
}
else
std::cout << "no more space\n";
}
else
{
std::cout << "else\n";

char buf[1000];
int rval;

bzero(buf, sizeof(buf));
if ((rval = read(i, buf, 999)) < 0)
perror("reading streaù ùessage");
else if (rval == 0)
std::cout << "ending connection\n";
else
printf("->>%s", buf);
//                    server.
write(i, "yo", 2);
}
}
}
}