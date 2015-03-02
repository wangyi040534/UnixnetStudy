#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/types.h>
#include <stdlib.h>
void str_echo(int connfd);
void main(int argc, char **argv)
{
	int listenfd, connfd;
	int childpid;
	int clilen;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9877);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);
	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*)&cliaddr,
				         &clilen);
		if((childpid = fork()) == 0)
		{
			printf("connect is established and enter child process\n");
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}

void str_echo(int connfd)
{
	int n;
	char buf[1024];
again:
	bzero(buf,1024);
	while((n = read(connfd, buf, 1024)) > 0)
	{
		write(connfd, buf, n);
		bzero(buf,1024);
	}
	if(n < 0)
		goto again;
	else if(n == 0)
		printf("str_echo:there is no data\n");
}
