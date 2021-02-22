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

//��̬��ʼ��
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

   
/*
       int pthread_mutex_destroy(pthread_mutex_t *mutex);
       int pthread_mutex_init(pthread_mutex_t *restrict mutex,
              const pthread_mutexattr_t *restrict attr);
              */

//posix �߳̿�ĺ��� �߳̿�
void *thread_routine(void* arg)
{
	int i = 0;

	ThreadInfo *tmp = (ThreadInfo *)arg;
//	pthread_detach(pthread_self());

	//�̻߳�ȡ��������
	pthread_mutex_lock(&mutex);
	printf("g_num:%d \n", g_num);
	sleep(1);
	printf("numid:%d \n", tmp->numId);
	pthread_mutex_unlock(&mutex);
	
	
	/*
	ThreadInfo *tmp = (ThreadInfo *)arg;
	for (i=0; i<nLoop; i++)
	{
		printf("�߳�ip:%d Ȧ��:%d numid:%d \n", pthread_self(), i , tmp->numId);
	}

	printf("���߳�˯��3��󡣡��������˳���\n");
	*/
	//usleep(100);
	
	pthread_exit(0);

} 

//���ۣ�return arg �� pthread_exit�����Ľ����������pthread_join �ӹ���
int main()
{
	
	int i = 0;
	nNum= 10;
	nLoop = 10;
	ThreadInfo tmpArray[200];
	
	 //pthread_mutex_init(&mutex, NULL);

	pthread_t tidArray[200];
	g_num = 11;
	
	printf("\n�������߳�����");
	scanf("%d", &nNum);
	
	
	printf("\n�������߳�Ȧ����");
	scanf("%d", &nLoop);
	
	
	for (i=0; i<nNum; i++)
	{
		//�������ڴ� ip+port
		tmpArray[i].numId = i+1;
		
		//tmp.numId = i; //��˵�� �̺߳�����ʹ����һ�����ϱ仯���ڴ�ռ䡣������
		pthread_create(&tidArray[i], NULL, thread_routine, (void *)&(tmpArray[i]));
	}
	
	for (i=0; i<nNum; i++)
	{
		pthread_join(tidArray[i], NULL); //�ȴ��߳̽���������
	}
	
	printf("����ҲҪ����1233\n");
	
	return 0;
}



