#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

//IPC���󡣡���Ϣ������linux�ں˸���־û������ǿ��Դ�linux�ں��л�ȡ ��ȡ��Ϣ���е���Ϣ���޸���Ϣ���У�

int main41()
{
	printf("sizeof(long):%d \n", sizeof(long));
	printf("sizeof(int):%d \n", sizeof(int));
	return 0;
}


struct msgbuf {
     long mtype;     /* message type, must be > 0 */
     char mtext[1024*10];  /* message data */
};

int main(int argc, char *argv[])
{
	int msgid;
	int ret = 0;
	
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <msg type> <msg len bytes> \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int type = atoi(argv[1]);
	int len = atoi(argv[2]);
	
	
	//rwx
	msgid = msgget(0x1234, 0666);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		if (errno == EEXIST)
		{
			printf("�����Լ���飬��������Ϣ���Ѿ�����\n");
		}
		
		perror("msgget err");
		return -1;
	}
	printf("msgid:%d \n", msgid);
	

	struct msgbuf buf;
	memset(&buf, 0, sizeof(struct msgbuf));
	buf.mtype = type;
	strcpy(buf.mtext, "1111222222222433333333sssssssssssss");
	
	
	ret = msgsnd(msgid, &buf, len, IPC_NOWAIT);
	if (ret < 0)
	{
		perror("msgsnd err");
		return -1;
	}
	return 0;
}


