
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

	char revbuf[1024] = {0};
	while (1)
	{
		int ret = read(conn, revbuf, sizeof(revbuf));
		if (ret == 0)
		{
			//����ڶ��Ĺ����У��Է��Ѿ��رգ�tcpipЭ��᷵��һ��0���ݰ�
			printf("�Է��ѹر�\n");
			exit(0);
		}
		else if (ret < 0)
		{
			
			perror("������ʧ��\n"); 
			exit(0);
		}
		//ssize_t write(int fd, const void *buf, size_t count);
		fputs(revbuf, stdout); //���������յ����ݣ���ӡ��Ļ
		write(conn, revbuf, ret); //�������˻ط����ģ��ط�һ���ı��ģ�
		
	}
	 
	return 0;

}
