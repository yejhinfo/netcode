#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
     pid_t pid;
	//注册信号 子进程退出 忽略信号，避免了僵尸进程
	signal(SIGCHLD, SIG_IGN);
	printf("befor fork pid:%d \n", getpid());
	int abc = 10;
	pid = fork(); //errno
	if (pid == -1)	
	{
		perror("tile");
		return -1;
	}
	//大于0是父进程（为了获取子进程的id号，所以让子进程的id为0）
	if (pid > 0)
	{
		abc ++;
		printf("parent: pid:%d \n", getpid());
		printf("abc: %d \n", abc);
		sleep(20);
	}
	//等于0是子进程
	else if (pid == 0)
	{
		abc ++;
		printf("child: %d, parent: %d \n", getpid(), getppid());
		printf("abc: %d \n", abc);
		//sleep(100);
	}

	printf("fork after....\n");
	return 0;
}