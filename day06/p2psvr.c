
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
//����ͨ�õĽṹ����Ϊbind����֧�ֶ���Э�飬������Ҫ�þ����ĸ�Э���ʱ������ǿ������ת������
	 struct sockaddr {
               sa_family_t sa_family;
               char        sa_data[14];
           }

//��������Ǿ����tcp/ip�ṹ ��man  7 ip��
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
	 addr.sin_port = htons(8888);//�����ض˿�װΪ���϶˿�
	 addr.sin_addr.s_addr = inet_addr("127.0.0.1");//������ipתΪ����ip
	if(bind(socketfd,(const struct sockaddr *) &addr,sizeof(addr))<0){
			perror("fun bind error\n");
			exit(0);
	}
	//һ��������listen������socketfd����ɱ����׽��֣����������������ӣ�ֻ�ܽ�������
	//���һ�ά���������У�һ���ǵȴ��������Ӷ��У�һ����������ӵĶ���
	//SOMAXCONN����֧�ֵ����������
	if(listen(socketfd,SOMAXCONN)<0){
			perror("fun listen error\n");
			exit(0);
	}
	struct sockaddr_in peeraddr;//�Է���ip��ַ
	socklen_t peerlen = sizeof(peeraddr);
	unsigned int conn = 0;
	//accpet �᷵��һ���µ����ӣ�����µ�������һ�������׽���
	conn = accept(socketfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if(conn == -1){
		    perror("fun socketfd error\n");
			exit(0);
	}
	//תΪ���ص�ip��ַ�Ͷ˿ں�
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));

	int pid = fork();
	
	if(pid>0){
		struct packet recvbuf;
		int n;
		while(1){
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
				//��ͻ���д����
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
