#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXLINE 1000
#define DEBUG 0


//global   declarations
int pid_child  =0;
int grand_child=0;
int parallel_id =0;
char CD[] = "cd";
char RUN[] = "run";
char PARALLEL[] = "parallel";
char CRON[] = "cron";


//declarations
char ** tokenize(char*);
//for run
void execute1(char*);
//for parallel
void cron (char*);

char** currtime();

void exec_file(char** );

//function defifniton for cron, incomplete
void cron (char* path) {
	//2d array to store the list of commands
	char** repeat = (char**)malloc (1000*sizeof(char *));
	
	
	FILE * batch;
	//Open the file
	batch =fopen ( path, "r");
	int  i =0;
	//if file sdosn't exists
	if (batch ==NULL) {
		printf ("given file doesnt exist \n");
	}
	else { //else
		//struct tm y2k;
		
		//char ** command;
		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		
		//reading line by line
		while ((read = getline(&line, &len, batch)) != -1) {
			repeat[i] = (char*)malloc (100*sizeof(char));
			strcpy(repeat[i++],line);
			//printf("%s\n",line);
			//printf("repeat -- %s\n",repeat[i-1] );
		}
	}

	char** timenow = (char**)malloc (5*sizeof(char*));
    timenow=currtime();
	
	int j=0,k=0;  
	 

	for(j=0;j<i;j++)
	{
		
		char** command = tokenize(repeat[j]);


		int flag=0;

		
		for(k=0;k<5;k++)
		{
			//printf("match found---1\n"); 
			printf("%s -- ", command[k]);
			printf("%s\n", timenow[k]);
			//printf("match found---2\n"); 
			if((!strcmp(command[k],"*")) ||  (!strcmp(command[k],timenow[k]))){
				flag=1;
				continue;
			}
			else 
			{
				flag=0;
				break;
			}
			
		}

		if(flag==1)
		{
			printf("match found\n");
		}
		else
		{
			printf("match not found\n");
		}

	}











	
	//freeing  the spaxe taken
	free(repeat);
}
		
		
/*
	return current time and date in an array.
*/

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



//changes directory with chdir command
void changedir(char* path) {
	int output = chdir(path);
	// if path dosen't exists
	if( output == -1) {
		printf("path doesn't exists\n");
	}
	else if(output == 0) {//else no use whatsoever except to debug
		//printf("path changed successfully, currently in directory : \n");
		//system("pwd");
		
	}
	
}

//kills child process with pid= pid_child
void killchild (int signum) {
	kill(pid_child, SIGINT);//used in run
}



//does the parallel execution of commands

void parallelize(char** tokens) {
	//space to store each command, albiet one by one
	char** job = (char**)malloc (1000*sizeof(char*));
	

	

	int i =1;
	int j =0;
	//pid of child and execvp treturn values
	int pid_child,exect;
	while(1) {
		//at the end of the total comand
		if(tokens[i] == NULL) {
			job[j] = NULL;
			//create child process
			pid_child = fork();
			if(pid_child ==0) {
				//if command - cd
				if( !strcmp(job[0],CD) ){
					changedir (job[1]);
					//break;
				}
				//else if command = run
				else if( !strcmp(job[0],RUN) ){
					execute1(job[1]);
					//break;
				}
				//else an anything else without cron
				else {
					exect = execvp ( job[0], job);
					printf("%s failed with error  : %d \n", job[0], exect);
					fflush(stdout);
					exit(0);
					//break;
				}
			}
			else break;
		}
		// if command is still not complete to execute
		// further adding and composing the comand to be executed
		else if (strcmp( tokens[i],":::")) {
			job[j] = strdup(tokens[i]); //making a copy
			j++;
		}
		// else execution
		else {
			job[j] = NULL;
			pid_child = fork();//child process
			// the below cases ar same as the first ocndition
			// the below cases ar same as the first ocndition
			if(pid_child ==0) {
				if( !strcmp(job[0],CD) ){
					changedir (job[1]);
				}
				else if( !strcmp(job[0],RUN) ){
					execute1(job[1]);
				}
				else {
					exec_file(job);
					exit(0);
				}
			}
			else j=0;
		}
		i++;
	}
	// freeing sopace allocated

	while(1)
	{
		if ( errno == ECHILD )
		{
			break;
		}
		wait();

	}
	

	free(job);	 

}










//executes run, if cd found among the commands, calls the changedir function
//similarly for parallel to parallelize
void execute1(char* path) { 
	FILE * batch;
	batch =fopen ( path, "r");
	
	// if no file
	if (batch ==NULL) {
		printf ("given file doesnt exist \n");
	}
	else {
		
		char ** command;
		char *line = NULL;
		size_t len = 0;
		ssize_t read;
		// reading line by line
		while ((read = getline(&line, &len, batch)) != -1) {
			//printf("Retrieved line of length %s :\n", &read);
			//printf("%s", line);
			// breaking the line into tokens
			command = tokenize(line);
			//if empty, do nothing
			if(command[0] == NULL) {
				break;
			}
			// if comand = cd
			if( !strcmp(command[0],CD) ){
				if (command[1] == NULL) 
				printf("no path supplied\n");
				else changedir( command[1]);
			}
			//else if command = run
			else if( !strcmp(command[0],RUN) ){
				if (command[1] == NULL) 
				printf("no path supplied\n");
				else
					execute1( command[1]);
			}
			//else if parallel
			else if (!strcmp(command[0],PARALLEL) ){
				if(command[1] == NULL)
				printf("no commands given\n");
				else {
					//printf("%s\n", input);
					parallelize(command);
				}
			}
			//else anything else ohter then cron
			else {
				
				//forking and making a child process
				// and executing it
				pid_child = fork();
				if( pid_child ==0)
				 {
					exec_file( command);
					exit(0);
				}
				else {
					//checks to kill child process if any forcibly from the
					//parent process
					signal (SIGINT, killchild);//killchild if any
					wait();
					signal (SIGINT , SIG_DFL);// kill parent process- jash
				}
			}
				
		}
		//free space allocated
		free(command);
	}
}


//execute1s all other commnads other then cd or run
void exec_file(char** tokens) {
	//similar as the last case of execute1
	int i = 0, flag1 =0, flag2 = 0, fd1 = -1, fd2 = -1, flag3= 0;
	int flag4 = 0, fd3 = -1, flag5 = 0;
	
	
		
		
		char** job = (char**)malloc (1000*sizeof(char*));
		char** job1 = (char**)malloc (1000*sizeof(char*));
				
				
		
		while(tokens[i] != NULL) {
			
			if(!strcmp(tokens[i],"<")) {
				int fd1;
				 fd1 = open(tokens[i+1], O_RDONLY) ;
				
				if(flag1==1)
				{
					perror(tokens[i+1]);
					exit(0);
				}	
				flag1 = 1;
				dup2(fd1,0);
				i++;
				

				continue;
			
			}
			
			if(!strcmp(tokens[i],">")) {
				
				
				int fd2;
				if ( (fd2 = open(tokens[i+1], O_CREAT |O_TRUNC | O_WRONLY, 0644)) < 0  || flag2==1) {
					perror(tokens[i+1]);
					exit(0);
				}
				flag2 = 1;
				
				dup2(fd2,1);
				i++;
				continue;
						
			}

			if(!strcmp (tokens[i], ">>")) {
				
				int fd3;
				if ( (fd3 = open(tokens[i+1], O_APPEND | O_RDWR)) < 0 || flag1==1 || flag2==1 || flag4==1) {
					perror(tokens[i+1]);
					exit(0);
				}

				flag4 = 1;
				//printf ("dfskjhfkjafdja %d  %s \n", fd3, tokens[i+1]);
				dup2(fd3,1);
				i++;
				continue;
			}
				 
			if(!strcmp (tokens[i], "&")) {
				flag5 = 1;
				break;
			}
			
			
			
			if(!strcmp(tokens[i],"|")) {
				flag3 = 1;
				break;
			}
			
			job[i] = strdup(tokens[i]); //making a copy
			
			i++;
		}
		
		if( flag3 == 1) {
			i=0;

			while (strcmp(tokens[i],"|")) {
				
				job[i] = strdup(tokens[i]);
				
				i++;
			}
			
			i++;

			

			int j = 0;
			while( tokens[i] != NULL) {
				
				job1[j] = strdup(tokens[i]);
				i++;
				j++;
			}


			if((!strcmp(job[0],"run")) || (!strcmp(job[0],"cd")) ||  (!strcmp(job1[0],"run")) || (!strcmp(job1[0],"cd")))
			{
				perror("invalid input");
				exit(0);
			}

		
			int pipefd[2];
			  int pid;

			  pipe(pipefd);

			  pid = fork();

				if (pid == 0){
 
					dup2(pipefd[1], 1);
					exec_file(job);   
				   
					close(pipefd[1]);

					exit(0);
				}
				else{
					dup2(pipefd[0], 0);  
					exec_file(job1);
					close(pipefd[0]);
					signal (SIGINT, killchild);//killchild if any
					wait();
					signal (SIGINT , SIG_DFL);// kill parent process- jash

				}



		}
		else if (flag5 == 1) {
			pid_child = fork();
			if (pid_child == 0) {
				if (!strcmp(job[0],RUN)) {
					execute1(job[1]);
					
				}
				else {
					//printf("%d\n", pid_child);
					exec_file(job);
					//printf("%s failed with error  : %d \n", job[0], exect);
				}
				exit(0);
			}
			else {
				printf("%d\n", pid_child);
			}
			
			
		}
				
		else{
			if (!strcmp(job[0],"exit") ){
				killpg(getpgrp(),SIGTERM);
			}
		

			int exect = execvp (job[0],job);
printf("---fdfdf111	--------\n");
			printf("---%d\n", exect);
			printf("%s failed with error  : %d \n", tokens[0], exect);
			//fflush(stdout);
			
			if ( flag1 == 1) {
				close(fd1);
			}
			 
			if ( flag2 == 1) {
				close(fd2);
			}
			
			if (flag4 ==1) {
				close(fd3);
			}
			
			
			exit(0);
		}
		
		free(job);
		free(job1);
		exit(0);	
	
	
}
	


void termchild(int signum)
{
	kill(grand_child, SIGTERM);
}


//main
int main(int argc, char** argv){

	//Setting the signal interrupt to its default function. 
	signal(SIGINT, SIG_DFL);

	//Allocating space to store the previous commands.
	int numCmds = 0;
	char **cmds = (char **)malloc(1000 * sizeof(char *));                                // free()  ????

	int printDollar = 1;

	char input[MAXLINE];
	char** tokens;

	int notEOF = 1;
	int i;

	FILE* stream = stdin;

	while(notEOF) { 
		if (printDollar == 1){ 
			printf("$ "); // the prompt
			fflush(stdin);																	//???????????????
		}

		char *in = fgets(input, MAXLINE, stream); 


		//taking input one line at a time




		//Checking for EOF
		if (in == NULL){
			if (DEBUG) printf("EOF found\n");												//??????????????
			exit(0);																		//ctrl + c : kill child
		}

		//add the command to the command list.
		cmds[numCmds] = (char *)malloc(sizeof(input));
		strcpy(cmds[numCmds++], input); 

		// Calling the tokenizer function on the input line    
		tokens = tokenize(input);	
		

		grand_child = fork();
		if( grand_child ==0)
		{
			


		//the next part check s if the comaand is a cd or a run and hence can be done 
		//in a single if else statement
		if (tokens[0] == NULL) continue;// if no command, not to crash
		else if (strcmp (tokens[0],CD) == 0) {// if cd
			if (tokens[1] == NULL) 
			printf("no path supplied\n");
			else changedir( tokens[1]);
		}
		else if(strcmp (tokens[0],RUN) == 0) {//if run
			if (tokens[1] == NULL) 
			printf("no path supplied\n");
			else
					
				 execute1( tokens[1]);
		}
		else if (!strcmp(tokens[0],PARALLEL) ) { //if parallel
			if(tokens[1] == NULL)
			printf("no commands given\n");
			else {
				//printf("%s\n", input);


				
					parallelize(tokens);
				}
			
		}
		else if (!strcmp(tokens[0],CRON) ) {//if cron
			if(tokens[1] == NULL)
			printf("no file given\n");
			else {
				//printf("%s\n", input);
				cron(tokens[1]);
				}
		}
		else {//else
			exec_file (tokens);
		}

		}
		else {
					//checks to kill child process if any forcibly from the
					//parent process
					signal (SIGINT, termchild);//killchild if any
					wait();
					signal (SIGINT , SIG_DFL);// kill parent process- jash
				}


	}
  
  
	
	// Freeing the allocated memory	
	free(tokens);
	return 0;
}

/*
*	the tokenizer function takes a string of chars and forms tokens out of it
*/
char ** tokenize(char* input){
	int i;
	int doubleQuotes = 0;
	
	char *token = (char *)malloc(1000*sizeof(char));
	int tokenIndex = 0;

	char **tokens;
	tokens = (char **) malloc(MAXLINE*sizeof(char**));
 
	int tokenNo = 0;
	
	for(i =0; i < strlen(input); i++){
		char readChar = input[i];
		
		if (readChar == '"'){
			doubleQuotes = (doubleQuotes + 1) % 2;
			if (doubleQuotes == 0){
				token[tokenIndex] = '\0';
				if (tokenIndex != 0){
					tokens[tokenNo] = (char*)malloc(sizeof(token));
					strcpy(tokens[tokenNo++], token);
					tokenIndex = 0; 
				}
			}
		}
		else if (doubleQuotes == 1){
			token[tokenIndex++] = readChar;
		}
		else if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0){
				tokens[tokenNo] = (char*)malloc(sizeof(token));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else{
			token[tokenIndex++] = readChar;
		}
	}
	
	if (doubleQuotes == 1){
		token[tokenIndex] = '\0';
		if (tokenIndex != 0){
			tokens[tokenNo] = (char*)malloc(sizeof(token));
			strcpy(tokens[tokenNo++], token);
		}
	}
	
	return tokens;
}
