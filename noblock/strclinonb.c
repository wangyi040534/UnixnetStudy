#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
int max(int a, int b)
	return a > b?a:b;
void str_cli(FILE *fp, int sockfd)
{
	int maxfdp1, val,stdineof;
	int nwritten;
	fd_set rset, wset;
	char to[1024], fr[1024];
	char *toiptr, *tooptr, *friptr, *froptr;
	val = fcntl(sockfd, F_GETFL, 0);//change file property
	fcntl(sockfd, F_SETFL, val | O_NONBLOCK);//set noblocking i/o socket
	val = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);//set noblocking stdin
	val = fcntl(STDOUT_FILENO, F_GETFL, 0);
	fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);//set noblocking stdout
	toiptr = tooptr = to;//initialize buffer pointers
	friptr = froptr = fr;
	stdineof = 0;
	maxfdp1 = max(max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
	for(;;){
		FD_ZERO(&rset);
		FD_ZERO(&wset);

	}
}
