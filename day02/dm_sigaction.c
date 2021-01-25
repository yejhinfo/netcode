
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


//演示信号从产生到抵达的整个过程
//信号的阻塞和解除阻塞综合实验
//设置信号阻塞和非阻塞，设置ctl+q来解除信号阻塞

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void handler(int sig);

void handler(int sig)
{
	if (sig == SIGINT)
		printf("recv a sig=%d\n", sig);
	else if (sig == SIGQUIT)
	{
		sigset_t uset;
		sigemptyset(&uset);
		sigaddset(&uset, SIGINT);
		//ctr + \ 用来接触  SIGINT 信号
		//解除阻塞
		sigprocmask(SIG_UNBLOCK, &uset, NULL);
	}
}

void printsigset(sigset_t *set)
{
	int i;
	for (i=1; i<NSIG; ++i)
	{
		if (sigismember(set, i))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}


/*
          struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/


//3 连续的按ctrl+c键盘，虽然发送了多个SIGINT信号，但是因为信号是不稳定的，只保留了一个。
//不支持排队
int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = handler;
	
	sigaction(SIGINT,&act,NULL);
	
	for (;;)
	{
		sleep(1);
	}
	return 0;
}



