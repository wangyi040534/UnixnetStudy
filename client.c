#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#define MAXN 16384
int tcp_connect(char *ip, char *port);
int main(int argc, char **argv)
{
	int i,j,fd,nchildren,nloops,nbytes;
	int pid;
	int n;
	char request[MAXN], reply[MAXN];
	if(argc != 4)
		printf("usgae:client<hostname or IPaddr><port><#children>"
				"<#loops/child><#bytes/request>\n");
	//nchildren = atoi(argv[3]);
	nchildren = 10;
	//nloops = atoi(argv[4]);
	//nbytes = atoi(argv[5]);
	nloops = 10;
	nbytes = atoi(argv[3]);
    snprintf(request, sizeof(request), "%d\n", nbytes);
	//printf("initialize successfully!\n");
	i = 0;
	for(i; i < nchildren; i++){
		if((pid = fork()) == 0){
            printf("%d child fork successfully!\n",i);
			j = 0;
			for(j;j < nloops; j++){
				printf("%d child %d loop is successful!\n", i, j);
				fd = tcp_connect(argv[1], argv[2]);
				write(fd, request, strlen(request));
				if((n = read(fd, reply, nbytes)) != nbytes)
					printf("server returned %d bytes", n);
				close(fd);
			}
			printf("child %d done\n",i);
			exit(0);
		}
	}
	while(wait(NULL) > 0)
		;
	//if(errno != ECHILD)
	//	printf("wait eror");
	exit(0);
}

int tcp_connect(char *ip, char *port)
{
    //printf("not connected\n");
	int sockfd, connfd;
	struct sockaddr_in  servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET,ip, &servaddr.sin_addr);
	if((connfd = connect(sockfd, (struct sockaddr*)&servaddr,
				        sizeof(servaddr))) != 0)
		printf("connect failed!\n");
    //printf("%d is connected",connfd);
	return connfd;
}
