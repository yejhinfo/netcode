#include <sys/types.h>
#include <unistd.h>


#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

/*
       #include <signal.h>

       typedef void (*sighandler_t)(int);

       sighandler_t signal(int signum, sighandler_t handler);
*/
void handler(int num)
{
	printf("recv num:%d \n", num);	
}
//信号异步通知事件 。。。。。（我的应用程序dm01）可以随时被信号机制中断。。。如果我我写了信号处理函数handler
//那么当信号发生时，会自动调用我写的函数。。。。
void main()
{
	//注册一个信号
	//SIGINT  是ctrl+c 会产生2号信号。。。 中断应用程序
	signal(SIGINT, handler);
	
	while(1)
	{
		pause();
	}
	
	printf("main...Begin\n");
		
}
