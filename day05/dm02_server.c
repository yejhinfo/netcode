
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

*/


int main(int argc, char const *argv[])
{
	int socketfd = 0;
	
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

	char revbuf[1024] = {0};
	while (1)
	{
		int ret = read(conn, revbuf, sizeof(revbuf));
		if (ret == 0)
		{
			//如果在读的过程中，对方已经关闭，tcpip协议会返回一个0数据包
			printf("对方已关闭\n");
			exit(0);
		}
		else if (ret < 0)
		{
			
			perror("读数据失败\n"); 
			exit(0);
		}
		//ssize_t write(int fd, const void *buf, size_t count);
		fputs(revbuf, stdout); //服务器端收到数据，打印屏幕
		write(conn, revbuf, ret); //服务器端回发报文（回发一样的报文）
		
	}
	 
	return 0;

}
