#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <signal.h>



int main(int argc, char const *argv[])
{

	fork();
	fork();
	fork();
	printf("fork after....\n");
	return 0;
}
