#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

//IPC���󡣡���Ϣ������linux�ں˸���־û������ǿ��Դ�linux�ں��л�ȡ ��ȡ��Ϣ���е���Ϣ���޸���Ϣ���У�



#define MsgMax 1024*10 
struct msgbuf {
     long mtype;     /* message type, must be > 0 */
     char mtext[MsgMax];  /* message data */
};

int main(int argc, char *argv[])
{
	int msgid;
	int ret = 0;
	int flag = 0;
	
	int type = 0;
	
	
	if (argc == 1)
	{
		fprintf(stderr, "Usage: %s <��Ϣ���� type 0 1 2 3 .. -1 -2 -3> <n ���� > \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 2)
	{
		type = atoi(argv[1]);
		flag = 0;
	}
	
	if (argc == 3)
	{
		type = atoi(argv[1]);
		flag = flag | IPC_NOWAIT;
	}
	
	
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
	//buf.mtype = 0; 
	
	//int n;
	//msgtyp=0��ʾ������Ϣ���еĵ�һ����Ϣ������
	//ret = msgrcv(msgid, &buf, MsgMax, 0, IPC_NOWAIT);
	ret = msgrcv(msgid, &buf, MsgMax, type, flag);
	if (ret < 0)
	{
		perror("msgsnd err");
		return -1;
	}
	buf.mtext[ret] = '\0';
	printf("mtext:%s \n", buf.mtext);
	
	return 0;
}
