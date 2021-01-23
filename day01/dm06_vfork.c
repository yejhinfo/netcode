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
    int ret;

	pid = vfork(); //errno
	if (pid == -1)	
	{
		perror("tile");
		return -1;
	}
	//大于0是父进程（为了获取子进程的id号，所以让子进程的id为0）
	if (pid > 0)
	{
		printf("parent: pid:%d \n", getpid());
	}
	//等于0是子进程
	else if (pid == 0)
	{
		printf("child: %d, parent: %d \n", getpid(), getppid());
		//printf("abc: %d \n", abc);
		//ret = execve("./hello",NULL,NULL);
		//通过一个应用程序执行应用
		char *const argv[] = {"ls","-l",NULL};
		ret = execve("/bin/ls",argv,NULL);
		
		if (ret == -1)
		{
			perror("execve: ");
		}
		printf("execve test\n");
		exit(0);

	}

	printf("fork after....\n");
	return 0;
}