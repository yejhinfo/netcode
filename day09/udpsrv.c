#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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

void echo_srv(int sock)
{
	char 		recvbuf[1024] = {0};
	struct 		sockaddr_in peeraddr;
	socklen_t	peerlen;
	int 		n;
	
	while (1)
	{
		peerlen = sizeof(peeraddr);
		memset(recvbuf, 0, sizeof(recvbuf));
		//
		n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&peeraddr, &peerlen);
		if (n == -1)
		{
			if (errno == EINTR)
				continue;
			
			ERR_EXIT("recvfrom");
		}
		else if (n > 0)
		{
			int ret = 0;
			fputs(recvbuf, stdout);
			//注意sendto需要指定对方的地址
			ret = sendto(sock, recvbuf, n, 0, (struct sockaddr*)&peeraddr, peerlen);

		}
	}

	close(sock);
}

int main(void)
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		ERR_EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8002);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
		
	//conn = accept

	echo_srv(sock);
	return 0;
}
