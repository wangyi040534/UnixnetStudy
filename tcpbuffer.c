#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
void
main(int argc, char **argv)
{
	int sock, getsockopt_return;
	int send_buff, recv_buff;
	socklen_t buff_size;
	send_buff = 0;
	recv_buff = 0;
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		printf("socket failed!\n");
    buff_size = sizeof(send_buff);
    if((getsockopt_return = getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
					        &send_buff, &buff_size)) < 0)
		printf("get send buffer failed!\n");
	else
		printf("the size of send buffer is %d\n", send_buff);

	buff_size = sizeof(recv_buff);
	getsockopt_return = 0;
    if((getsockopt_return = getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
					        &recv_buff, &buff_size)) < 0)
		printf("get recv buffer failed!\n");
	else
		printf("the size of recv buffer is %d\n", recv_buff);
}
