#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

//IPC对象。。消息队列是linux内核给你持久化（我们可以从linux内核中获取 获取消息队列的信息、修改消息队列）



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
		fprintf(stderr, "Usage: %s <消息类型 type 0 1 2 3 .. -1 -2 -3> <n 阻塞 > \n", argv[0]);
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
	//buf.mtype = 0; 
	
	//int n;
	//msgtyp=0表示接受消息队列的第一条消息。。。
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
