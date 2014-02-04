#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <iostream>
using namespace std;



char** currtime()
{
	char** timenow = (char**)malloc (5*sizeof(char*));
	time_t mytime;
    mytime = time(NULL);
    char* time1 = ctime(&mytime);

    struct tm *ltm = localtime(&mytime);
    int i=0;

    //cout<<ltm->tm_min<<endl;
   //char name = static_cast<char>(ltm->tm_min);
   

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
    char** timenow = (char**)malloc (5*sizeof(char*));
    timenow=currtime();
    cout<<timenow[0]<<endl;
      cout<<timenow[1]<<endl;
      cout<<timenow[2]<<endl;
      cout<<timenow[3]<<endl;
      cout<<timenow[4]<<endl;

    return 0;
}