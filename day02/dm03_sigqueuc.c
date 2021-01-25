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

typedef struct Teacher
{
	char name[64];
	int age ;
}Teacher;



//用新的回到函数接受数据。。。。  
void myHandle_forsigaction(int signum, siginfo_t *s_t, void *p)
{
	//printf("recv a sig=%d data=%d data=%d\n", sig, info->si_value.sival_int, info->si_int);
		int myint = 0;
		printf("recv signum : %d \n", signum);
		myint = s_t->si_value.sival_int;
		printf("%d %d \n", myint, s_t->si_int );
	
}


int main(int argc, char *argv[])
{
	pid_t 	pid;
	int ret = 0;
	
	struct sigaction act;
	act.sa_sigaction = myHandle_forsigaction;
	sigemptyset(&act.sa_mask);
	
	//如果父进程的T回调函数， 准备接受额外数据
	/*
	 * SA_SIGINFO，当设定了该标志位时，
	 * 表示信号附带的参数可以被传递到信号处理函数中，
	 * 因此，应该为sigaction结构中的sa_sigaction指定处理函数，
	 * 而不应该为sa_handler指定信号处理函数，
	 * 否则，设置该标志变得毫无意义。
	 * 即使为sa_sigaction指定了信号处理函数，
	 * 如果不设置SA_SIGINFO，信号处理函数同样不能得到信号传递过来的数据，
	 * 在信号处理函数中对这些信息的访问都将导致段错误（Segmentation fault）。
	 * */
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &act, NULL) < 0)
		ERR_EXIT("sigaction error");
		
	pid = fork();
	
	if (pid == -1)
	{
		printf("fork err...\n");
		return 0;
	}
	
	if (pid == 0)
	{
	   int i = 0;
	   union sigval  mysigval;
	   mysigval.sival_int = 222;	
	
		//kill(getppid(), SIGINT);
		//带额外数据
		for (i=0; i<10; i++)
		{
			ret  = sigqueue(getppid(), SIGINT, mysigval);
			if (ret != 0)
			{
				printf("sigqueue .....\n");
				exit(0);
			}
			else
			{
				printf("sigqueue...successs\n");
				sleep(2);
			}
		}

	}
	else if (pid > 0)
	{
		for (;;)
			pause();
	}

	return 0;
}

void handler(int sig)
{
	printf("recv a sig=%d\n", sig);
	sleep(5);
}
