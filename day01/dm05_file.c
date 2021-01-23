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
#include <unistd.h>



int main(int argc, char const *argv[])
{
     pid_t pid;
     int fd;
	//注册信号 子进程退出 忽略信号，避免了僵尸进程
	signal(SIGCHLD, SIG_IGN);
	printf("befor fork pid:%d \n", getpid());
	fd = open("./1.txt",O_RDWR);
	if(fd < 0){
		perror("open");
		return -1;
	}


	pid = fork(); //errno
	if (pid == -1)	
	{
		perror("tile");
		return -1;
	}
	//大于0是父进程（为了获取子进程的id号，所以让子进程的id为0）
	if (pid > 0)
	{
		printf("parent: pid:%d \n", getpid());
		write(fd,"parent",6);
		close(fd);//父子进程打开的是同一个fd所以在父子进程中需要关闭两次
		//sleep(20);
	}
	//等于0是子进程
	else if (pid == 0)
	{
		printf("child: %d, parent: %d \n", getpid(), getppid());
		write(fd,"child",5);
		//sleep(100);
		close(fd);
	}

	printf("fork after....\n");
	return 0;
}