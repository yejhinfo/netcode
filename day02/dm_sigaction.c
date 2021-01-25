
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


//��ʾ�źŴӲ������ִ����������
//�źŵ������ͽ�������ۺ�ʵ��
//�����ź������ͷ�����������ctl+q������ź�����

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
		//ctr + \ �����Ӵ�  SIGINT �ź�
		//�������
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


//3 �����İ�ctrl+c���̣���Ȼ�����˶��SIGINT�źţ�������Ϊ�ź��ǲ��ȶ��ģ�ֻ������һ����
//��֧���Ŷ�
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



