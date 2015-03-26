#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include<sys/types>
#define MAXN 16384
void web_child(int sockfd);
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
void web_child(int sockfd)
{
	int ntowrite, nread;
	char line[MAXN], result[MAXN];
	for(;;){
		if((nread = readline(sockfd, line, MAXN)) == 0)
			return;
		ntowrite = atol(line);
		if((ntowrite <= 0) || (ntowrite > MAXN))
			printf("client request for %d bytes", ntowrite);
		write(sockfd, result, ntowrite);
	}
}
