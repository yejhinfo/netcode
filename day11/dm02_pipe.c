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

int main21(void )
{
	int pipefd[2];
	pid_t pid;
	if (pipe(pipefd) == -1 )	
	{
		printf("pipe() err..\n");	
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("fork err..\n");
		return -1;
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		//复制文件描述符pipefd[1]，给标准输出，言外之意：execlp的ls命令输出到管道中
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		execlp("ls", "ls", NULL);
		//如果替换新的进程印象失败，则会执行下面一句话	
		sprintf(stderr, "execute the cmd ls err..\n");
		exit(0);	
	
	
	} 
	else if (pid > 0 )
	{
		int len = 0; 
		char buf[100] = {0};
		close(pipefd[1]);
		//复制文件描述符pipefd[0]，给标准输入，言外之意：execlp的wc命令从管道中读
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		//len = read(pipefd[0], buf, 100);
		execlp("wc", "wc", "-w", NULL);
		printf("len:%d, buf:%s \n", len , buf);

		//close(pipefd[0]);
	}

	wait(NULL);
	printf("parent ..quit\n");
	return 0;

}


int main(int argc, char *argv[])
{
	close(0); //关闭标准输入
	open("makefile", O_RDONLY); //makefile文件变成标准输入
	close(1);//关闭标准输出
	open("makefile2", O_WRONLY | O_CREAT | O_TRUNC, 0644); //maifle2变成标准输出

	execlp("cat", "cat", NULL); //替换进程印象后，执行cat命令
	
	//cat命令 从标准输入中按行读，紧接着写到标准输出

	return 0;
	
}