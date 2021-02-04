#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

struct packet
{
	int len;
	char buf[1024];
};

//1一次全部读走 //2次读完数据 //出错分析 //对方已关闭
//思想：tcpip是流协议，不能保证1次读操作，能全部把报文读走，所以要循环读指定长度的数据。
//按照count大小读数据，
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。

//@ssize_t：返回读的长度 若ssize_t<count 读失败失败
//@buf：接受数据内存首地址
//@count：接受数据长度
ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nread == 0) //若对方已关闭
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

//1一次全部读走 //2次读完数据 //出错分析 //对方已关闭
//思想：tcpip是流协议，不能1次把指定长度数据，全部写完 
//按照count大小写数据
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。
//@ssize_t：返回写的长度 -1失败
//@buf：待写数据首地址
//@count：待写长度

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char*)buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}

void do_service(int conn)
{
	struct packet recvbuf;
	int n;
    while (1)
	{
        memset(&recvbuf, 0, sizeof(recvbuf));
        int ret = readn(conn, &recvbuf.len, 4); //读包头 4个字节
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < 4)
		{
			printf("client close\n");
			break;
		}
		
		n = ntohl(recvbuf.len);
		ret = readn(conn, recvbuf.buf, n); //根据长度读数据
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < n)
		{
			printf("client close\n");
			break;
		}
		
	    fputs(recvbuf.buf, stdout);
	   
	    writen(conn, &recvbuf, 4+n);  //注意写数据的时候，多加4个字节
	}
}

int main(void)
{
	int listenfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
/*	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)*/
		ERR_EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");*/
	/*inet_aton("127.0.0.1", &servaddr.sin_addr);*/

	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;
	while (1)
	{
		if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept");

		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		}
		else
			close(conn);
	}
	
	return 0;
}
