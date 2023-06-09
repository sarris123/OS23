/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
void* jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
jobs fg_job;
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */
	struct sigaction Ctrl_C;
	struct sigaction Ctrl_Z;
	
	sigset_t maskC;
	sigset_t maskZ;
	
	sigfillset(&maskC);
	sigfillset(&maskZ);
	
	Ctrl_C.sa_flags = 0;
	Ctrl_Z.sa_flags = 0;
	
	Ctrl_C.sa_flags = maskC;
	Ctrl_Z.sa_flags = maskZ;
	
	
	Ctrl_C.sa_handler = &ctrl_c_handler;
	Ctrl_Z.sa_handler = &ctrl_z_handler;
	
	if((sigaction(SIGKILL,&Ctrl_C,NULL))||(sigaction(SIGSTOP,&Ctrl_Z,NULL))){
		perror("smash error: sigaction failed");
		return 1;
	}
	

	/************************************/

	/************************************/
	
    	while (1)
    	{
		fg_job.process_ID = -1;
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		//if(!ExeComp(lineSize)) continue; 
					// background command	
	 	//if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

