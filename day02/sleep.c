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
		//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
	signal(SIGINT, handler);
	
	do 
	{
		n = sleep(n);  //sleep�ǿ��ж�˯�ߣ��ý���˯�� 
		printf("Ҫ��˯������������\n");
	} while(n > 0);
	
	
	//wait();
	printf("sleep ....����");

}