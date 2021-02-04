
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
     
/*
       int socket(int domain, int type, int protocol);
       */
       
 /*
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);

			//man 2 bind
         struct sockaddr {
             sa_family_t sa_family;
             char        sa_data[14];
         }
         
         //man 7 ip
           struct sockaddr_in {
             sa_family_t    sin_family; // address family: AF_INET 
             u_int16_t      sin_port;   // port in network byte order 
             struct in_addr sin_addr;   // internet address 
         };

         //Internet address. 
         struct in_addr {
             u_int32_t      s_addr;     //address in network byte order 
         };

*/

      //int listen(int sockfd, int backlog);
      
      //accept 接受一个新的连接 ，这个新的连接是一个主动套接字
    /*   int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
      	       On success, accept() returns a non-negative integer that is a descriptor for the accepted socket.   On  error,
       -1 is returned, and errno is set appropriately.
     int conn = 0;



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

int main()
{
	int  sockfd = 0;
	signal(SIGUSR1,sighandler_t);
	sockfd =  socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	
	struct sockaddr_in  srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8888);//转线上端口
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //转线上ip
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //绑定本机的任意一个地址 
	
    if ( connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0)
    {
    	perror("fun socket\n");
		exit(0);
    }

	int pid = fork();
	if(pid>0){			
			struct packet sendbuf;		
			int n;
			while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
			{
				//向客户端写数据
				n = strlen(sendbuf.buf);
				sendbuf.len = htonl(n);	
				writen(sockfd, &sendbuf, 4+n);
				memset(&sendbuf, 0, sizeof(sendbuf));
			}

	}
	else if(pid==0){
		struct packet recvbuf;
		memset(&recvbuf, 0, sizeof(recvbuf));
		while (1)
		{		int n;
				int ret = readn(sockfd, &recvbuf.len, 4);//这里调用的是&recvbuf.len做传出参数，给recvbuf.len赋值
                if (ret == -1)
                        ERR_EXIT("read");
                else if (ret < 4)
                {
                        printf("client close\n");
                        break;
                }
                
                n = ntohl(recvbuf.len);
                ret = readn(sockfd, recvbuf.buf, n);
                if (ret == -1)
                        ERR_EXIT("read");
                else if (ret < n)
                {
                        printf("client close\n");
                        break;
                }
				fputs(recvbuf.buf, stdout);
				memset(&recvbuf, 0, sizeof(recvbuf));
		} 	
		close(sockfd);
		kill(getppid(),SIGUSR1);
		exit(0);
			
	}
	
	
	return 0; 	
}






 