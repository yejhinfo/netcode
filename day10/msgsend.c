#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

//IPC对象。。消息队列是linux内核给你持久化（我们可以从linux内核中获取 获取消息队列的信息、修改消息队列）

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
			printf("我们自己检查，，，，消息队列不存在\n");
		}
		if (errno == EEXIST)
		{
			printf("我们自己检查，，，，消息队已经存在\n");
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


