#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>



char** currtime()
{
	char** timenow = (char**)malloc (5*sizeof(char*));
	time_t mytime;
    mytime = time(NULL);
    char* time1 = ctime(&mytime);

    struct tm *ltm = localtime(&mytime);
    int i=0;

    timenow[0] = (char*)malloc (5*sizeof(char));
    timenow[1] = (char*)malloc (5*sizeof(char));
    timenow[2] = (char*)malloc (5*sizeof(char));
	timenow[3] = (char*)malloc (5*sizeof(char));
	timenow[4] = (char*)malloc (5*sizeof(char));

    char* t=(char*) malloc (5* sizeof(char));
    snprintf( t,5,"%d",ltm->tm_min);
    strcpy(timenow[0],t);
 		
 	snprintf( t,5,"%d",ltm->tm_hour);
    strcpy(timenow[1],t);

    snprintf( t,5,"%d",ltm->tm_mday);
    strcpy(timenow[2],t);

    snprintf( t,5,"%d",ltm->tm_mon + 1);
    strcpy(timenow[3],t);

    snprintf( t,5,"%d",ltm->tm_wday);
    strcpy(timenow[4],t);
    
      
     return timenow;
}

int main(void)
{
   printf("match found\n");
    return 0;
}