
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


       uint32_t htonl(uint32_t hostlong);

       uint16_t htons(uint16_t hostshort);

       uint32_t ntohl(uint32_t netlong);

       uint16_t ntohs(uint16_t netshort);


		int inet_aton(const char *cp, struct in_addr *inp);
		in_addr_t inet_addr(const char *cp);
		char *inet_ntoa(struct in_addr in);


*/


int main(int argc, char const *argv[])
{
	unsigned int date = 0x12345678;
	char *p = &date;
	int i = 0;
	for(i=0;i<=3;i++){
		printf("%x",p[i]);
	}
	printf("\n");

	printf("-------------------\n");
	uint32_t netdate = htonl(date);
	p = &netdate;
		for(i=0;i<=3;i++){
		printf("%x",p[i]);
	}
	printf("\n");
	printf("-------------------\n");

	printf("inet_addr output: %u\n",inet_addr("192.168.0.103"));

	struct in_addr inp;
	inet_aton("192.168.0.103", &inp);
	printf("inet_aton output: %u\n",inp.s_addr);

	printf("inet_ntoa output: %s\n",inet_ntoa(inp));
	return 0;
}
