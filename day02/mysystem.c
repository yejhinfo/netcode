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


/*    如果无法启动shell运行命令；出现不能执行system调用的其他错误时返回-1。如果system能够顺利执行，返回那个命令的退出码。*/

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int my_system(const char *command);

int main(int argc, char *argv[])
{
	//system("dir | wc -w");
	my_system("ls -l | wc -w");
	return 0;
}

int my_system(const char *command){
	pid_t pid;
	int status;
	if(command==NULL){
		return 1;
	}
	//创建不了子进程 statu将被设为-1
	if((pid = fork()) < 0) status = -1;
	//子进程中启动shell
	else if(pid == 0){
		execl("/bin/sh","sh","-c",command,NULL);
		exit(127);
	}
	//父进程将等待回收子进程
	else{
		//防止信号中断子进程
		while(waitpid(pid,&status,0)<0){
			if(errno == EINTR) continue;

			status = -1;
			break;
		}
	}

	return status;
}