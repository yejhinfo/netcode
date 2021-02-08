
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
#include "commsocket.h"
typedef struct _SckHandle
{
	int sockfd;
	int connfd;
	int contime;
	int sendtime;
	int revtime;
}SckHandle;

//函数声明
//客户端环境初始化
int sckCliet_init(void **handle, char *ip, int port, int contime, int sendtime, int revtime)
{
	int 	ret = 0;
	if (handle == NULL ||contime<0 || sendtime<0 || revtime<0)
	{
		ret = Sck_ErrParam;
		printf("func sckCliet_init() err: %d, check  (handle == NULL ||contime<0 || sendtime<0 || revtime<0)\n", ret);
		return ret;
	}
	
	if (ip == NULL ||port<0 || port>65535 )
	{
		ret = Sck_ErrParam;
		printf("func sckCliet_init() err: %d, check  (ip == NULL ||port<0 || port>65535 )\n", ret);
		return ret;
	}
	
	SckHandle *tmp = (SckHandle *)malloc(sizeof(SckHandle));
	if (tmp == NULL)
	{
		ret = Sck_ErrMalloc;
		printf("func sckCliet_init() err: malloc %d\n", ret);
		return ret;
	}
	
	tmp->contime = contime;
	tmp->sendtime = sendtime;
	tmp->revtime = revtime;
	
	//
	
	int sockfd;
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		ret = errno;
		printf("func socket() err:  %d\n", ret);
		return ret;
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(ip);
	
	int connfd; 
	connfd = connect(sockfd, (struct sockaddr*) (&servaddr), sizeof(servaddr));
	if (connfd < 0)
    {
    	ret = errno;
		printf("func connect() err:  %d\n", ret);
		return ret;
    }
    tmp->sockfd = sockfd;
    tmp->connfd  = connfd;
  	
	return ret;
}

//客户端发送报文
int sckClient_send(void *handle, unsigned char *data, int datalen)
{
	
	return 0;
}

//客户端端接受报文
int sckClient_rev(void *handle, unsigned char *out, int *outlen)
{
	return 0;
}

// 客户端环境释放 
int sckClient_destroy(void *handle)
{
	return 0;
}
