
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

 int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

DESCRIPTION
       The sigaction() system call is used to change the action taken by a process on receipt of a specific signal.

       signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.

       If act is non-null, the new action for signal signum is installed from act.  If oldact is non-null, the previous
       action is saved in oldact.

       The sigaction structure is defined as something like

              struct sigaction {
                  void (*sa_handler)(int);
                  void (*sa_sigaction)(int, siginfo_t *, void *);
                  sigset_t sa_mask;
                  int sa_flags;
                  void (*sa_restorer)(void);
              }
*/

/*

   int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

DESCRIPTION
       sigprocmask()  is  used  to  change the signal mask, the set of currently blocked signals.  The behaviour of the
       call is dependent on the value of how, as follows.

              SIG_BLOCK
                     The set of blocked signals is the union of the current set and the set argument.

              SIG_UNBLOCK
                     The signals in set are removed from the current set of blocked signals.  It is legal to attempt to
                     unblock a signal which is not blocked.

              SIG_SETMASK
                     The set of blocked signals is set to the argument set.

*/

/*
       #include <signal.h>

       int sigemptyset(sigset_t *set);

       int sigfillset(sigset_t *set);

       int sigaddset(sigset_t *set, int signum);

       int sigdelset(sigset_t *set, int signum);

       int sigismember(const sigset_t *set, int signum);
       */
       
       
      /*   
       int sigqueue(pid_t pid, int sig, const union sigval value);

DESCRIPTION
       sigqueue() sends the signal specified in sig to the process whose PID is given in pid.  The permissions required
       to send a signal are the same as for kill(2).  As with kill(2), the null signal (0) can be used to  check  if  a
       process with a given PID exists.

       The  value argument is used to specify an accompanying item of data (either an integer or a pointer value) to be
       sent with the signal, and has the following type:

         union sigval {
             int   sival_int;
             void *sival_ptr;
         };
*/
void  myhandle(int num)
{
	printf("recv sig num:%d \n", num);
	if(num == SIGUSR1){
		sigset_t bset;
	    sigemptyset(&bset);
	    sigaddset(&bset, SIGINT);
		sigaddset(&bset, SIGRTMIN);
		sigprocmask(SIG_UNBLOCK,&bset,NULL);
	}
}

int main(int argc, char const *argv[])
{
	pid_t 	pid;
	
	struct sigaction act;
	act.sa_handler = myhandle;
	act.sa_flags = SA_SIGINFO;
	
	//注册非实时信号处理函数
	if(sigaction(SIGINT,&act,NULL)<0){
		ERR_EXIT("sigaction SIGINT");
	}
	//注册实时信号处理函数
	if(sigaction(SIGRTMIN,&act,NULL)<0){
		ERR_EXIT("sigaction SIGRTMIN");
	}
	//注册用户自定义信号
	if(sigaction(SIGUSR1,&act,NULL)<0){
		ERR_EXIT("sigaction SIGUSR1");
	}

	//把实时和非实时都添加到本进程的阻塞状态字中
    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
	sigaddset(&bset, SIGRTMIN);
	sigprocmask(SIG_BLOCK,&bset,NULL);

	pid = fork();
	if(pid == -1){
		ERR_EXIT("fork error");
	}

	if(pid == 0){
		int ret =0;
		int i = 0;
		union sigval value;
		value.sival_int = 2021;
		//发三次不稳定信号
		for(i=0;i<3;i++){
			ret = sigqueue(getppid(),SIGINT,value);
			if(ret != 0){
				ERR_EXIT("sigqueue error");
				exit(0);
			}
			else{
				printf("success believeable\n");
			}
		}
		value.sival_int = 2020;
		//发三次稳定信号
		for(i=0;i<3;i++){
			ret = sigqueue(getppid(),SIGRTMIN,value);
			if(ret != 0){
				ERR_EXIT("sigqueue error\n");
				exit(0);
			}
			else{
				printf("success unbelieveable\n");
			}
		}

		kill(getppid(),SIGUSR1);
	
	}

	while(1){
		pause();
	}

	return 0;
}


