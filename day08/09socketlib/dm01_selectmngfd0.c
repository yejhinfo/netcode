#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
/*

       #include <sys/select.h>

       //According to earlier standards 
       #include <sys/time.h>
       #include <sys/types.h>
       #include <unistd.h>

       int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);
*/


int main01(void) 
{
	   fd_set rfds;
	   struct timeval tv;
	   int retval;
	
	   /* Watch stdin (fd 0) to see when it has input. */
	   FD_ZERO(&rfds);
	   
	   while (1)
	   {
	   		FD_SET(0, &rfds);
	
		   /* Wait up to five seconds. */
		   tv.tv_sec = 2;
		   tv.tv_usec = 0;
		
		   retval = select(1, &rfds, NULL, NULL, &tv);
		   /* Don't rely on the value of tv now! */
		
		   if (retval == -1)
		       perror("select() -1");
		   else if (retval)
		   	{
		   		 printf("Data is available now.\n");
		   	}
		      
		       /* FD_ISSET(0, &rfds) will be true. */	
		   else
		       printf("No data within five seconds.\n");
		}


  	 	return 0;
}

int
main(void) 
{
	   fd_set rfds;
	   struct timeval tv;
	   int retval;
	
	   /* Watch stdin (fd 0) to see when it has input. */
	   FD_ZERO(&rfds);
	   FD_SET(0, &rfds);
	
	   /* Wait up to five seconds. */
	   tv.tv_sec = 3;
	   tv.tv_usec = 0;
	   
	  // while (1)
	   {
		   	   retval = select(1, &rfds, NULL, NULL, &tv);
		   /* Don't rely on the value of tv now! */
		
		   if (retval == -1)
		       perror("select()");
		   else if (retval)
		   	{
		   		if (FD_ISSET(0, &rfds)) 
		   			{
		   				printf("检测出0号文件描述符发生变化\n");
		   				exit(0);
		   			}
		   	}
		       //printf("Data is available now.\n");
		       /* FD_ISSET(0, &rfds) will be true. */
		   else
		       printf("No data within 3 seconds.\n");
	   	
	   }
	
	
	
	   return 0;
}
