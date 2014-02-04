#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>


int main()
{
	int i;
	i=changedir( "..");
	printf("%d\n",i );
	return 0;
}