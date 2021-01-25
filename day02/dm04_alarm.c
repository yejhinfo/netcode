#include <sys/types.h>
#include <unistd.h>


#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>


typedef struct _Teacher
{
	int age;
	int num;	
}Teacher;

Teacher g_t;
char *p = NULL;

void printfGlobTeacher()
{
	printf("g_t.age:%d \n", g_t.age);
	printf("g_t.num:%d \n", g_t.num);
	//p = malloc(100);
}

void  myhandle(int num)
{
	printf("recv signal id num : %d \n", num);
	//kill -alram ` ps -aux | grep 01aram | grep -v vi | awk '{print $2}' ` 
	printfGlobTeacher();
	alarm(1);
}

int main(void)
{
	Teacher t1, t2;
	t1.age = 30;
	t1.num = 30;
	t2.age = 40;
	t2.num = 40;
	printf("main ....begin\n");
	//注册信号处理函数
	if (signal(SIGALRM, myhandle) == SIG_ERR)
	{
		perror("func signal err\n");
		return 0;
	} 
	//间接递归
	//myhandle----->alarm=====>myhandle
	alarm(1);
	while(1) 
	{
		//pause();
		g_t = t1;
		
		g_t = t2;
		
		//printf("pause return\n");
	}
	return 0;
}