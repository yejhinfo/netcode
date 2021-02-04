
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
     
int main()
{
	int  sockfd = 0;
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
	   
	
	char revbuf[1024] = {0};
	char sendbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		//�����д����
		write(sockfd, sendbuf, strlen(sendbuf)); //�������˻ط���Ϣ
		
		//�ӷ�����������
		read(sockfd, revbuf, sizeof(revbuf));	
		
		//
		fputs(revbuf, stdout); //�ӷ������յ����ݣ���ӡ��Ļ
		
		memset(revbuf, 0, sizeof(revbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
		
	}
	
	close(sockfd);
	
	return 0; 	
}






 