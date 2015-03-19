#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<poll.h>
#include<sys/stropts.h>
#include<limits.h>//for OPEN_MAX
#define INFTIM -1
#define OPEN_MAX 1024
int main(int argc, char** argv)
{
	int i, maxi, listenfd, connfd, sockfd;
	int nready;// client[FD_SETSIZE];
	int n;
	//fd_set rset, allset;
	char buf[1024];
	socklen_t clilen;
        struct pollfd client[OPEN_MAX];
	struct sockaddr_in cliaddr,servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9877);
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(listenfd, 10);
	//maxfd = listenfd;
        client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	maxi = 0;
		for(i = i; i < OPEN_MAX; i++)
		{
			client[i].fd = -1;
		}
		//FD_ZERO(&allset);
	//	FD_SET(listenfd, &allset);
			
	for(;;)
	{
	//rset = allset;//structure assignment
	//nready = select(maxfd+1, &rset, NULL, NULL,NULL);
	/*if(FD_ISSET(listenfd, &rset))//new client connection
	{
	    clilen = sizeof(cliaddr);
	    connfd = accept(listenfd, (struct sockaddr*)&cliaddr,
			&clilen);
	    for(i = 0; i < FD_SETSIZE;i++)
		if(client[i] < 0){
		   client[i] = connfd;
		   break;
		}
		if(i == FD_SETSIZE)
		printf("too many clients");
	    FD_SET(connfd, &allset);//add new descriptor to set
	    if(connfd > maxfd)
	        maxfd = connfd;//for select parameter maxfd1
	    if(i > maxi)
	        maxi = i;//max index in client[] array
	    if(--nready <= 0)
	        continue;	
	    }
	    for(i = 0; i <= maxi;i++){
	    if((sockfd = client[i]) < 0)
		continue;
	    if(FD_ISSET(sockfd,&rset)){
	     if((n = read(sockfd, buf, 1024)) == 0){
		 close(sockfd);
		 FD_CLR(sockfd, &allset);
		 client[i] = -1;
	 }
	  else
	write(sockfd, buf ,n);
	if(--nready <= 0)
	break;//no more readable descriptors
	}
	}*/
	nready = poll(client, maxi + 1, INFTIM);
	if(client[0].revents & POLLRDNORM){//new client connection
	    clilen = sizeof(cliaddr);
	    connfd = accept(listenfd, (struct sockaddr*)&cliaddr,
	                    &clilen);
	    for(i = 1; i < OPEN_MAX; i++)
	        if(client[i].fd < 0){
		    client[i].fd = connfd;//save descriptor
		    break;
		}
	    if(i == OPEN_MAX)
	        printf("too many clients");
	    client[i].events = POLLRDNORM;
	    if(i > maxi)
	        maxi = i;//max index in client[] array
	    if(--nready <= 0)
	        continue;//no more descriptor
	}
	for(i =1; i <= maxi; i++){//check all clients for data
	    if((sockfd = client[i].fd) < 0)
	        continue;
	    if(client[i].revents & (POLLRDNORM | POLLERR)){
	        if((n = read(sockfd, buf, 1024)) < 0){
		    close(sockfd);
		    client[i].fd = -1;
		}else if(n == 0){
		    close(sockfd);
		    client[i].fd = -1;
		}else
		    write(sockfd, buf,n);
		if(--nready <= 0)
		break;//mo more readable descriptors
	    }
	}
    }
}
