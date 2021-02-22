#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>


typedef struct _ThreadInfo
{
	char 	ip[100];
	int 	port;
	char name[64];
	int age ;
	int numId;
}ThreadInfo;

int g_num = 0;

int 	nNum, nLoop;

  // int pthread_mutex_lock(pthread_mutex_t *mutex);
  //     int pthread_mutex_trylock(pthread_mutex_t *mutex);
   //    int pthread_mutex_unlock(pthread_mutex_t *mutex);

//静态初始化
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

   
/*
       int pthread_mutex_destroy(pthread_mutex_t *mutex);
       int pthread_mutex_init(pthread_mutex_t *restrict mutex,
              const pthread_mutexattr_t *restrict attr);
              */

//posix 线程库的函数 线程库
void *thread_routine(void* arg)
{
	int i = 0;

	ThreadInfo *tmp = (ThreadInfo *)arg;
//	pthread_detach(pthread_self());

	//线程获取锁。。。
	pthread_mutex_lock(&mutex);
	printf("g_num:%d \n", g_num);
	sleep(1);
	printf("numid:%d \n", tmp->numId);
	pthread_mutex_unlock(&mutex);
	
	
	/*
	ThreadInfo *tmp = (ThreadInfo *)arg;
	for (i=0; i<nLoop; i++)
	{
		printf("线程ip:%d 圈数:%d numid:%d \n", pthread_self(), i , tmp->numId);
	}

	printf("子线程睡眠3秒后。。。。。退出了\n");
	*/
	//usleep(100);
	
	pthread_exit(0);

} 

//结论：return arg 和 pthread_exit（）的结果都可以让pthread_join 接过来
int main()
{
	
	int i = 0;
	nNum= 10;
	nLoop = 10;
	ThreadInfo tmpArray[200];
	
	 //pthread_mutex_init(&mutex, NULL);

	pthread_t tidArray[200];
	g_num = 11;
	
	printf("\n请输入线程数：");
	scanf("%d", &nNum);
	
	
	printf("\n请输入线程圈数：");
	scanf("%d", &nLoop);
	
	
	for (i=0; i<nNum; i++)
	{
		//读共享内存 ip+port
		tmpArray[i].numId = i+1;
		
		//tmp.numId = i; //这说明 线程函数在使用了一个不断变化的内存空间。。。。
		pthread_create(&tidArray[i], NULL, thread_routine, (void *)&(tmpArray[i]));
	}
	
	for (i=0; i<nNum; i++)
	{
		pthread_join(tidArray[i], NULL); //等待线程结束。。。
	}
	
	printf("进程也要结束1233\n");
	
	return 0;
}



