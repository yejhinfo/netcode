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

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int main21(void )
{
	int pipefd[2];
	pid_t pid;
	if (pipe(pipefd) == -1 )	
	{
		printf("pipe() err..\n");	
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("fork err..\n");
		return -1;
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		//�����ļ�������pipefd[1]������׼���������֮�⣺execlp��ls����������ܵ���
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		execlp("ls", "ls", NULL);
		//����滻�µĽ���ӡ��ʧ�ܣ����ִ������һ�仰	
		sprintf(stderr, "execute the cmd ls err..\n");
		exit(0);	
	
	
	} 
	else if (pid > 0 )
	{
		int len = 0; 
		char buf[100] = {0};
		close(pipefd[1]);
		//�����ļ�������pipefd[0]������׼���룬����֮�⣺execlp��wc����ӹܵ��ж�
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		//len = read(pipefd[0], buf, 100);
		execlp("wc", "wc", "-w", NULL);
		printf("len:%d, buf:%s \n", len , buf);

		//close(pipefd[0]);
	}

	wait(NULL);
	printf("parent ..quit\n");
	return 0;

}


int main(int argc, char *argv[])
{
	close(0); //�رձ�׼����
	open("makefile", O_RDONLY); //makefile�ļ���ɱ�׼����
	close(1);//�رձ�׼���
	open("makefile2", O_WRONLY | O_CREAT | O_TRUNC, 0644); //maifle2��ɱ�׼���

	execlp("cat", "cat", NULL); //�滻����ӡ���ִ��cat����
	
	//cat���� �ӱ�׼�����а��ж���������д����׼���

	return 0;
	
}