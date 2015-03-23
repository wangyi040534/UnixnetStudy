#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/select.h>
//#define FD_SETSIZE 1024
int main(int argc, char** argv)
{
		int i, maxi, maxfd, listenfd, connfd, sockfd;
		int nready, client[FD_SETSIZE];
		int n;
		fd_set rset, allset;
		char buf[1024];
		socklen_t clilen;
		struct sockaddr_in cliaddr,servaddr;
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(9877);
		bind(listenfd,
		     (struct sockaddr*)&servaddr, sizeof(servaddr));
		listen(listenfd, 10);
		maxfd = listenfd;
		maxi = -1;
		for(i = 0; i < FD_SETSIZE; i++)
		{
				client[i] = -1;
		}
		FD_ZERO(&allset);
		FD_SET(listenfd, &allset);
		for(;;)
		{
				rset = allset;//structure assignment
				nready = select(maxfd+1, &rset, NULL, NULL,NULL);
                if(FD_ISSET(listenfd, &rset))//new client connection
				{
					clilen = sizeof(cliaddr);
					connfd = accept(listenfd,
					                (struct sockaddr*)&cliaddr,
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
				for(i = 0; i <= maxi;i++){//check all clients for 
						                  //data
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
				}
		}

}
