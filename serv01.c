#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include<sys/types>
int tcp_listen(char *ip, char *port, socklen_t *addr);
int main(int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	void sig_chld(int), sig_int(int), web_child(int);
	socklen_t clilen, addrlen;
	struct sockaddr *cliaddr;
	if(argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if(argc == 3)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	else
		printf("usage: serv01<host><port>");


}
int tcp_listen(char *ip, char *port, socklen_t * addr)
{
	int sockfd, connfd;
	struct sockaddr_in  servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET,*ip, &servaddr.sin_addr);
	connfd = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	return connfd;
}
