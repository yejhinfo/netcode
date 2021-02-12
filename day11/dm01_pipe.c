#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

/*
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
*/
int main(int argc, char const *argv[])
{
	int pipefd[2],ret =0;
	ret = pipe(pipefd);
	if(ret < 0){
		ERR_EXIT("pipe");
	}
	int pid = 0;
	char buf[1024];
	pid = fork();
	if(pid==0){
		close(pipefd[0]);
		write(pipefd[1],"2333",4);
		exit(0);
	}else if(pid > 0){
		close(pipefd[1]);
		read(pipefd[0],buf,sizeof(buf));
		printf("%s\n",buf);

	}else{
		ERR_EXIT("fork()");
	}

	return 0;
}
