#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#define MAXN 16384
static int nchildren;
static pid_t *pids;
//int sig_int(int signo);
pid_t child_make(int i, int listenfd, int addrlen);
int tcp_listen(char *ip, char *port, int addr);
int main(int argc, char **argv)
{
	int listenfd ,i;
	socklen_t addrlen;
//	void sig_int(int);
	if(argc == 3)
		listenfd = tcp_listen(NULL, argv[1], addrlen);
	else if (argc == 4)
		listenfd = tcp_listen(argv[1], argv[2], addrlen);
	else
		printf("usage: serv02[<host>]<port#><#children>");
	nchildren = atoi(argv[argc -1]);
	pids = calloc(nchildren, sizeof(pid_t));//what's function calloc
	i = 0;
	for(i; i < nchildren; i++)
		pids[i] = child_make(i, listenfd, addrlen);//make child process
//	signal(SIGINT, sig_int);
	for(;;)
		pause();
}

/*int sig_int(int signo)
{
	int i = 0;
//	void pr_cpu_time(void);
	for(i; i < nchildren; i++)
		kill(pids[i], SIGTERM);
	while(wait(NULL) > 0)
		;//wait for all children
//	pr_cpu_timr();
	exit(0);

}*/
int tcp_listen(char *ip, char *port, int addr)
{
	int listenfd, connfd;
	struct sockaddr_in  servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(port));
	if( ip == NULL)
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        inet_pton(AF_INET,*ip, &servaddr.sin_addr);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, addr);
	return listenfd;
}

pid_t child_make(int i, int listenfd, int addrlen)
{
	pid_t pid;
	void child_main(int i, int listenfd, int addrlen);
	if((pid = fork()) > 0)
		return (pid);
	child_main(i, listenfd, addrlen);//never returns
}
void child_main(int i, int listenfd, int addrlen)
{
	int connfd;
	void web_child(int);
	socklen_t clilen;
	struct sockaddr *cliaddr;
	cliaddr =  malloc(addrlen);
	printf("child %ld starting\n", (long)getpid());
	for(;;){
		clilen = addrlen;
		connfd = accept(listenfd, cliaddr, &clilen);
		web_child(connfd);
		close(connfd);
	}
}

void web_child(int sockfd)
{
	int ntowrite, nread;
	char line[MAXN], result[MAXN];
	for(;;){
			if((nread = read(sockfd, line, MAXN)) == 0)
			return;
		ntowrite = atol(line);
		if((ntowrite <= 0) || (ntowrite > MAXN))
			printf("client request for %d bytes", ntowrite);
		write(sockfd, line, ntowrite);
	}
}
