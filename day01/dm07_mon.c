#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

int mydaemon(int nochild, int noclose){
	pid_t pid;
	pid = fork(); //errno
	int i = 0;
	if (pid == -1)	
	{
		perror("tile");
		exit(0);
	}
	//大于0是父进程（为了获取子进程的id号，所以让子进程的id为0）
	if (pid > 0)
	{
		exit(0);
	}

	printf("pid :%d\n",getpid());
	pid = setsid();
	if(nochild == 1) chdir("/");
	
	if(noclose == 1){
		for (i=0; i<3; ++i) close(i);

	}
 //相当于把0号文件描述符指向/dev/null
	open("/dev/null", O_RDWR); //fd文件描述符fd-0的文件描述符指向  -16
	dup(0); //把0号文件描述符 赋值给空闲的文件描述符 1
	dup(0); //把0号文件描述符 赋值给空闲的文件描述符 2


	if(pid == -1){
		perror("setsid: ");
		exit(0);
	}
    for(;;)

	printf("fork after....\n");
	return 0;
}

int main(int argc, char const *argv[])
{
 	mydaemon(1,1);
}