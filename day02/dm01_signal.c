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
//�ź��첽֪ͨ�¼� �������������ҵ�Ӧ�ó���dm01��������ʱ���źŻ����жϡ������������д���źŴ�����handler
//��ô���źŷ���ʱ�����Զ�������д�ĺ�����������
void main()
{
	//ע��һ���ź�
	//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
	signal(SIGINT, handler);
	
	while(1)
	{
		pause();
	}
	
	printf("main...Begin\n");
		
}
