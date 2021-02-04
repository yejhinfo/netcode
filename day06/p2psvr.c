
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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>



/*
 int socket(int domain, int type, int protocol);
 AF_INET             IPv4 Internet protocols          ip(7)
SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based byte  streams.   An  out-of-band  data
                       transmission mechanism may be supported.


	int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);
//这是通用的结构，因为bind函数支持多种协议，所以需要用具体哪个协议的时候在用强制类型转换即可
	 struct sockaddr {
               sa_family_t sa_family;
               char        sa_data[14];
           }

//下面这个是具体的tcp/ip结构 （man  7 ip）
  struct sockaddr_in {
               sa_family_t    sin_family; 
               in_port_t      sin_port;   
               struct in_addr sin_addr;  
           };


           struct in_addr {
               uint32_t       s_addr;   
           };

 int listen(int sockfd, int backlog);  

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int getsockopt(int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen);



      
	typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);



       int kill(pid_t pid, int sig);




*/

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)


struct packet
{
	int len;//int是4个字节的，放在头部这样就读头部的四个字节就可以顺利的读出长度了
	char buf[1024];
};
//1一次全部读走 //2次读完数据 //出错分析 //对方已关闭
//思想：tcpip是流协议，不能保证1次读操作，能全部把报文读走，所以要循环读指定长度的数据。
//按照count大小读数据，
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。

//@ssize_t：返回读的长度 若ssize_t<count 读失败失败
//@buf：接受数据内存首地址
//@count：接受数据长度
ssize_t readn(int fd,void *buf, size_t count){
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*)buf;

	while(nleft > 0){
		if((nread = read(fd,bufp,nleft))<0){
			if(errno == EINTR){
				continue;
			return -1;
			}
		}
		else if(nread==0){
			return count -nleft;
		}

		bufp  += nread;
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
ssize_t writen(int fd, const void *buf, size_t count){
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


void sighandler_t(int num){
	printf("rev num: %d",num);
	exit(0);
}

int main(int argc, char const *argv[])
{
	int socketfd = 0;
	signal(SIGUSR1,sighandler_t);
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd==-1){
		perror("fun socketfd error\n");
		exit(0);
	}
	int opval =1;
	if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR,&opval, sizeof(opval))<0){
			perror("fun setsockopt error\n");
			exit(0);
	}
	 struct sockaddr_in addr;
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(8888);//将本地端口装为线上端口
	 addr.sin_addr.s_addr = inet_addr("127.0.0.1");//将本地ip转为线上ip
	if(bind(socketfd,(const struct sockaddr *) &addr,sizeof(addr))<0){
			perror("fun bind error\n");
			exit(0);
	}
	//一但调用了listen函数，socketfd将变成被动套接字，不能主动建立连接，只能接受连接
	//并且会维护两个队列，一个是等待王朝链接队列，一个是完成链接的队列
	//SOMAXCONN可以支持的最大连接数
	if(listen(socketfd,SOMAXCONN)<0){
			perror("fun listen error\n");
			exit(0);
	}
	struct sockaddr_in peeraddr;//对方的ip地址
	socklen_t peerlen = sizeof(peeraddr);
	unsigned int conn = 0;
	//accpet 会返回一个新的连接，这个新的连接是一个主动套接字
	conn = accept(socketfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if(conn == -1){
		    perror("fun socketfd error\n");
			exit(0);
	}
	//转为本地的ip地址和端口号
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));

	int pid = fork();
	
	if(pid>0){
		struct packet recvbuf;
		int n;
		while(1){
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
		}
		close(conn);
        kill(pid,SIGUSR1);
		exit(0);
	}
	else if(pid==0){
			struct packet sendbuf;		
			int n;
			while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
			{
				//向客户端写数据
				n = strlen(sendbuf.buf);
				sendbuf.len = htonl(n);	
				writen(conn, &sendbuf, 4+n);
				memset(&sendbuf, 0, sizeof(sendbuf));
			}
	}

	close(socketfd);
	close(conn);
	return 0;



}
