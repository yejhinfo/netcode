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

//1һ��ȫ������ //2�ζ������� //������� //�Է��ѹر�
//˼�룺tcpip����Э�飬���ܱ�֤1�ζ���������ȫ���ѱ��Ķ��ߣ�����Ҫѭ����ָ�����ȵ����ݡ�
//����count��С�����ݣ�
//����ȡ�ĳ���ssize_t<count ˵��������һ�����������Է��ѹرա�

//@ssize_t�����ض��ĳ��� ��ssize_t<count ��ʧ��ʧ��
//@buf�����������ڴ��׵�ַ
//@count���������ݳ���
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
		else if (nread == 0) //���Է��ѹر�
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

//1һ��ȫ������ //2�ζ������� //������� //�Է��ѹر�
//˼�룺tcpip����Э�飬����1�ΰ�ָ���������ݣ�ȫ��д�� 
//����count��Сд����
//����ȡ�ĳ���ssize_t<count ˵��������һ�����������Է��ѹرա�
//@ssize_t������д�ĳ��� -1ʧ��
//@buf����д�����׵�ַ
//@count����д����

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
        int ret = readn(conn, &recvbuf.len, 4); //����ͷ 4���ֽ�
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < 4)
		{
			printf("client close\n");
			break;
		}
		
		n = ntohl(recvbuf.len);
		ret = readn(conn, recvbuf.buf, n); //���ݳ��ȶ�����
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < n)
		{
			printf("client close\n");
			break;
		}
		
	    fputs(recvbuf.buf, stdout);
	   
	    writen(conn, &recvbuf, 4+n);  //ע��д���ݵ�ʱ�򣬶��4���ֽ�
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
