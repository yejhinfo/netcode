
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
      
      //accept ����һ���µ����� ������µ�������һ�������׽���
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
	int len;//int��4���ֽڵģ�����ͷ�������Ͷ�ͷ�����ĸ��ֽھͿ���˳���Ķ���������
	char buf[1024];
};
//1һ��ȫ������ //2�ζ������� //������� //�Է��ѹر�
//˼�룺tcpip����Э�飬���ܱ�֤1�ζ���������ȫ���ѱ��Ķ��ߣ�����Ҫѭ����ָ�����ȵ����ݡ�
//����count��С�����ݣ�
//����ȡ�ĳ���ssize_t<count ˵��������һ�����������Է��ѹرա�

//@ssize_t�����ض��ĳ��� ��ssize_t<count ��ʧ��ʧ��
//@buf�����������ڴ��׵�ַ
//@count���������ݳ���
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

//1һ��ȫ������ //2�ζ������� //������� //�Է��ѹر�
//˼�룺tcpip����Э�飬����1�ΰ�ָ���������ݣ�ȫ��д�� 
//����count��Сд����
//����ȡ�ĳ���ssize_t<count ˵��������һ�����������Է��ѹرա�
//@ssize_t������д�ĳ��� -1ʧ��
//@buf����д�����׵�ַ
//@count����д����
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
	srvaddr.sin_port = htons(8888);//ת���϶˿�
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //ת����ip
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //�󶨱���������һ����ַ 
	
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
				//��ͻ���д����
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
				int ret = readn(sockfd, &recvbuf.len, 4);//������õ���&recvbuf.len��������������recvbuf.len��ֵ
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






 