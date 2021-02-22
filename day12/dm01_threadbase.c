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



void *thread_routine(void* arg)
{
	int i = 0;
	//sleep(3);
	Teacher *p = NULL;
	p = (Teacher *)arg;
	printf("%d\n",p->age);

	for (i=0; i<20; i++)
	{
		printf("B");
		fflush(stdout);
	}
    pthread_exit(NULL);
	return NULL;
} 
int main(int argc, char const *argv[])
{
    
    printf("hello....\n");
	Teacher t1 = {"2333",32};
	pthread_t tid;
    pthread_create(&tid, NULL, thread_routine, &t1);
	//sleep(1);
    pthread_join(tid, NULL );
   
    return 0;
}
