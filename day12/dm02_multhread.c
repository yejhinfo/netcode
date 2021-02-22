#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>
 /*
 int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

   int pthread_join(pthread_t thread, void **retval);

 */

typedef struct Teacher
{
	char name[64];
	int  age;
}Teacher;

int nNum ,nLoop;

void *thread_routine(void* arg)
{
	int i = 0;
	//sleep(3);

	for (i=0; i<nLoop; i++)
	{
		printf("id;%d , loop:%d\n",pthread_self(),i);
		fflush(stdout);
	}
    pthread_exit(NULL);
	return NULL;
} 
int main(int argc, char const *argv[])
{
    
	pthread_t tidArray[200];
	nNum = 10;
	nLoop = 10;
	int i = 0;
	printf("input nNUm:%d\n",nNum);
	scanf("%d",&nNum);

	printf("input nLoop:%d\n",nLoop);
	scanf("%d",&nLoop);


	for(i = 0;i < nNum;i++){
		pthread_create(&tidArray[i], NULL, thread_routine, NULL);
		//sleep(1);
	}
	for (i=0; i<nNum; i++)
	{
		pthread_join(tidArray[i], NULL); //等待线程结束。。。
	}

    
	
   
    return 0;
}
