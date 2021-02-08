#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include "sckutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void handle(int signum)
{
	int pid = 0;
	printf("recv signum:%d \n", signum);
	
	//避免僵尸进程
	while ((pid = waitpid(-1, NULL, WNOHANG) ) > 0)
	{
		printf("退出子进程pid%d \n", pid);
		fflush(stdout);
	} 
}

int main(void)
{
	int pronum = 0;
	int loop = 0;
	int i =0;
	//signal(SIGCHLD, handle);
	
	printf("\nplease enter process num: ");
	scanf("%d", &pronum);
	
	printf("\nplease enter loop num:");
	scanf("%d", &loop);
	
	for (i=0; i<pronum; i++)
	{
		int j = 0;
		int pid = 0;
		pid = fork();
		if (pid == 0)
		//if (1)
		{
			int sock;
			if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
				ERR_EXIT("socket");
		
			struct sockaddr_in servaddr;
			memset(&servaddr, 0, sizeof(servaddr));
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(8001);
			servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		
		
			int ret = connect_timeout(sock, &servaddr, 5);
			if (ret == -1 && errno == ETIMEDOUT)
			{
				printf("timeout...\n");
				return 1;
			}
			else if (ret == -1)
				ERR_EXIT("connect_timeout");
		
			struct sockaddr_in localaddr;
			socklen_t addrlen = sizeof(localaddr);
			if (getsockname(sock, (struct sockaddr*)&localaddr, &addrlen) < 0)
				ERR_EXIT("getsockname");
		
			printf("ip=%s port=%d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));
			
			char sendbuf[1024] = {0};
			char recvbuf[1024] = {0};
			printf("loop:%d\n", loop);
			for (j=0; j<loop; j++)
			{
				sprintf(sendbuf, "sendbufdata process:%d loop:%d\n", i, j);
				//printf("aaaaaaaaaa\n");
				writen(sock, sendbuf, strlen(sendbuf));
				//printf("bbbbbbbbbbbbb\n");
				int ret = readline(sock, recvbuf, sizeof(recvbuf));
				//printf("ccccccccccccc\n");
		        if (ret == -1)
		                ERR_EXIT("readline");
		        else if (ret == 0)
		        {
	                printf("对方已关闭，所以client close\n");
	                close(sock);
	                break;
		        }
				//printf("dddddddddddddddd\n");
				fputs(recvbuf, stdout);
				fflush(stdout);
				memset(sendbuf, 0, sizeof(sendbuf));
				memset(recvbuf, 0, sizeof(recvbuf));
			}
			printf("process:%d 测试完毕退出\n", i);
			close(sock);
			exit(0);
		}
		else if (pid > 0)
		{
			printf("父进程\n");
		}
		else
		{
			printf("失败\n");
		}
	}
	while(1)
	sleep(11);
	//避免僵尸进程
	int mypid;
	/*
	while ((mypid = waitpid(-1, NULL, WNOHANG) ) > 0)
	{
		printf("退出子进程 mypid %d \n", mypid);
		fflush(stdout);
	} */
	
	return 0;
}
