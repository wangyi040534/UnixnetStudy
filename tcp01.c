#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

void str_cli(FILE *fp, int sockfd);
void main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in  servaddr;
	if(argc !=1)
	{
		printf("usage:tcpcli<IPaddress>");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	char *str;
	str = "127.0.0.1";
	inet_pton(AF_INET, str, &servaddr.sin_addr);
	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);
	exit(0);
}

void str_cli(FILE *fp, int sockfd)
{
	char sendline[1024], recvline[1024];
	while(fgets(sendline, 1024,fp) != 0)
		{
		write(sockfd, sendline, strlen(sendline));
		bzero(sendline,1024);
		if(read(sockfd, recvline, 1024) ==0)
		{
		printf("str_cli:server terminated prematurely");
		exit(1);
		}
		fputs(recvline, stdout);
		bzero(recvline,1024);
		}
	printf("there is no input data\n");
}
