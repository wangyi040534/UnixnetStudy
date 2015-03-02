#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
void str_cli(FILE *fp, int sockfd);
int max(int a, int b);
void main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in  servaddr;
	if(argc !=2)
	{
		printf("usage:tcpcli<IPaddress>");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);
	exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
	int maxfdp1;
	fd_set rset;
	FD_ZERO(&rset);
	char sendline[1024], recvline[1024];

	for(;;)
	{
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		select(maxfdp1, &rset, NULL, NULL, NULL);
		if(FD_ISSET(socket,&rset))
		{
		    if(read(sockfd, recvline, 1024) ==0)
		    {
		        printf("str_cli:server terminated prematurely");
		        exit(1);
		    }
			fputs(recvline, stdout);
		}
		if(FD_ISSET(fileno(fp),&rset))
		{
			if(fgets(sendline,1024,fp) == NULL)
				return;
			write(sockfd,sendline,strlen(sendline));
			bzero(sendline, 1024);
		}
	}

//	while(fgets(sendline, 1024,fp) != 0)
//		{
//		write(sockfd, sendline, strlen(sendline));
//		bzero(sendline,1024);
//		if(read(sockfd, recvline, 1024) ==0)
//		{
//		printf("str_cli:server terminated prematurely");
//		exit(1);
//		}
//		fputs(recvline, stdout);
//		}
//	printf("there is no input data\n");
}

int max(int a, int b)
{
	return a > b ?a:b;
}
