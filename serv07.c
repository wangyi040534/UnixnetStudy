#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
int tcp_listen(char *ip, char *port, socklen_t *addr);
//void web_child(int sockfd);
void thread_make(int i);
typedef struct{
	pthread_t thread_tid;//thread ID
	long thread_count;//connections handled
}Thread;

int listenfd, nthreads;
socklen_t  addrlen;
Thread *tptr;
pthread_mutex_t mlock;
int
main(int argc, char **argv)
{
	int i;
	struct sockaddr *cliaddr;
	if(argc == 3)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else if(argc == 4)
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	else
		printf("usage: serv01<host><port>");
	nthreads = atoi(argv[argc - 1]);
	tptr = calloc(nthreads, sizeof(Thread));
	for(i = 0; i < nthreads; i++)
		thread_make(i);
	for(; ; )
		pause();
}
int tcp_listen(char *ip, char *port, socklen_t *addr)
{
	int sockfd, connfd;
	struct sockaddr servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(port));
	inet_pton(AF_INET,*ip, &servaddr.sin_addr);
	connfd = connect(sockfd, &servaddr, sizeof(servaddr));
	return sockfd;
}
void thread_make(int i)
{
	void *thread_main(void *);
	pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *)&i);
	return;
}
void *thread_main(void *arg)
{
	int connfd;
	void web_child(int);
	socklen_t clilen;
	struct sockaddr *cliaddr;
	cliaddr = malloc(addrlen);
	printf("thread %d starting\n", (int)arg);
	for(; ; ){
		clilen = addrlen;
		pthread_mutex_lock(&mlock);
		connfd = accept(listenfd, cliaddr, &clilen);
		pthread_mutex_unlock(&mlock);
		tptr[(int)arg].thread_count++;
		web_child(connfd);
		close(connfd);
}
void web_child(int sockfd)
{
//	int ntowrite,
	int nread;
	char line[1024];
	for(;;){
		while(nread = readline(sockfd, line, 1024));
		//ntowrite = atol(line);
		//if((ntowrite <= 0) || (ntowrite > 1024))
		if((nread <= 0) || (nread > 1024))
			printf("client request for %d bytes", nread);
		write(sockfd, line, nread);
}
