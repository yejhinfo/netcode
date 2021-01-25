#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

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


void handler(int num)
{
	printf("recv num:%d \n", num);	
	if (num == SIGQUIT)
	{
		//exit(0);
	}

}

int main(void)
{

	int n = 100;
	pid_t 	pid;
	printf("main ....begin\n");
		//SIGINT  是ctrl+c 会产生2号信号。。。 中断应用程序
	signal(SIGINT, handler);
	
	do 
	{
		n = sleep(n);  //sleep是可中断睡眠，让进程睡够 
		printf("要给睡够。。。。。\n");
	} while(n > 0);
	
	
	//wait();
	printf("sleep ....结束");

}