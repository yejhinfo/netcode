#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>


int procnum = 10;
int loopnum = 100;
void testFun(int loopnum){
	printf("loopnum : %d\n",loopnum);
}

int main(int argc, char const *argv[])
{


    printf("please input procnum: \n");
    scanf("%d",&procnum);

    printf("please input loopnum: \n");
    scanf("%d",&loopnum);

    pid_t pid;
    int i = 0;
    int j = 0;

    for(i=0;i<procnum;i++){
    	pid = fork();
    	if(pid == 0){
    		for(j=0; j<loopnum;j++){
    			testFun(j);
    		}
    		exit(0);//子进程执行完就直接退出了，这样就可以保证不会出现子进程创建子进程
    	}
    }


	printf("fork after....\n");
	return 0;
}