#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>


//\n可以起到刷新缓冲区的操作，如果加了\n，那么用_exit也可以输出

/*
	atexit可以注册终止处理程序，ANSI C规定最多可以注册32个终止处理程序。
	终止处理程序的调用与注册次序相反

*/

void bye1(void){
	printf("bye1...\n");
}

void bye2(void){
	printf("bye2.....\n");
}

int main(int argc, char const *argv[])
{
	atexit(bye1);
	atexit(bye2);
	printf("hello");

	//exit(0)是c库函数，
	//exit(0);
	abort();
	return 0;
}

