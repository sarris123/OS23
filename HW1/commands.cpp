//		commands.c
//********************************************
#include "commands.h
#include "signals.h"
#include "job.h"
#include <string.h>
#include <stdbool.h>
#define MAX_JOB 100

using namespace std;

//Global variables
char* previous_path = NULL;
extern vector<jobs> job_list; 
extern jobs fg_job;


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
	char* cmd;
	int pid = -1;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") )   ///do again!!!!
	{
		
		if(num arg != 1){
			printf("smash error: cd: too many arguments\n");
			return 1;
			//illegal_cmd = TRUE;
		}
		char* curr_path =  get_current_dir_name();
			if(curr_path == NULL){
				perror("smash error: get_current_dir_name failed");
				return 1;
			} 
		else if(args[1] == "-"){
			if(previous_path == NULL){
				printf("smash error: cd: OLDPWD not set\n");
				return 1;
			}
			else{
				
				if(chdir(previos_path) == -1){
					perror("smash error: chdir failed");
					return 1;
				}else{
					previous_path = curr_path;
					return 0;
				}     
				
			}
		}
		else{
		    previous_path =  get_current_dir_name();
			if(previous_path == NULL){
				perror("smash error: get_current_dir_name failed");
				return 1;
			}
			if(chdir(args[1]) == -1){
				perror("smash error: chdir failed");
				return 1;
			}else{
				return 0;
			}
		}
		return 1;
		
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		
		char* path = get_current_dir_name();  
		if(path == NULL){
			perror("smash error: get_current_dir_name failed");
			return 1;
		}
		printf("%s\n",path);
		return 0;
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		//not needed
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		//remove all finished job
		JobDone(job_list); //implement later
		for(int i=0;i<job_list.size();i++){
			double current_time = time(NULL);
			if(job_list[i].status == STOPPED || job_list[i].status == BG ){
				if(job_list[i].status == STOPPED){
					printf("[%d] %s : %d %d secs (stopped)\n",job_list[i].job_id,job_list[i].cmd,job_list[i].process_ID,difftime(current_time,jobs[i].time););
				}
				else if (jobs[i].status == BG){
					printf("[%d] %s : %d %d secs\n ",job_list[i].job_id,job_list[i].cmd,job_list[i].process_ID,difftime(current_time,jobs[i].time););
				}

		}
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		pid = getpid();                    //always successful
		printf("smash pid is %d\n",pid);
		return 0;
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		//remove all finished jobs
		JobDone(job_list);
		//checking for number and validity of arguments
		if(num_arg != 2 || args[1].substr(0,1) != "-"){
			printf("smash error: kill: invalid arguments\n");
			return 1;
		}
		int signum = atoi(args[1]+1);
		int job-id = atoi(args[2]);
		string signal_name = get_sig_name(signum);
		//check atoi success
		if(job-id == 0 || signum == 0 || arg[2] == 0 || arg[1] == 0){
			printf("smash error: kill: invalid arguments\n");
			return 1;
		}
		//checking if job-id exists and getting its pid
		pid = FindJobPID(job_list,job-id); //implement later
		if(pid == -1 || pid == 0){
			printf("smash error: kill: job-id %d does not exist\n",job-id);
			return 1;
		}
		int ret_val = kill(pid,signum);
		if(ret_val == -1){
			perror("smash error: kill failed");
			return 1;
		}
		else{
			printf("signal number %d was sent to pid %d\n",signum,pid);
			//auto it = find_if(job_list.begin(),job_list.end(),[](const jobs& j) { return j.job_id == job-id; });
			auto it = find_job_index(job_list,job_id);  //implement later aligator !!!!!!
			
			if(signal_name == "SIGKILL" || signal_name == "SIGTERM"){
				job_list.erase(it); //remove job from list
			}
			if(signal_name == "SIGCONT" && (*it).status == STOPPED){
				(*it).status == BG;
			}
			if((signal_name == "SIGTSTP"||signal_name == "SIGSTOP")||signal_name == "SIGSTP") &&(*it).status == BG ){
				(*it).status == STOPPED;
			}
			
			return 0;
		}



	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{

		//remove all finished jobs
		JobDone(job_list);
		if(num_arg > 1 || num_arg < 0){
			printf("smash error: fg: invalid arguments\n");
			return 1;
		}
		if(num_arg == 0){
			int max = job_list.size();
			if(max == 0){
				printf("smash error: fg: jobs list is empty\n");
				return 1;
			}
			max = max -1;  //calculate index of last element whic is the element of the max job_id
			//max = job_list[max].job_id;  
			
			//print info
			printf("%s : %d\n",job_list[max].cmd,job_list[max].process_ID);
			//sending SIGCONT signal to process
			if(job_list[max].status == STOPPED){
				int ret = kill(job_list[max].process_ID,SIGCONT);
				if(ret == -1){
					perror("smash error: kill failed");
					return 1;
				}
			}
			fg_job = job_list.back();   //implement an operator&=
			pid = jobs[max].process_ID;
			//waiting for command to finished
			if( waitpid(pid,NULL,WUNTRACED) == -1 ){
				perror("smash error: waitpid failed");
				return 1;
			}
			else{
				//remove job from list
				job_list.erase(job_list.back());
				fg_job.process_ID = -1;	
				return 0;

			}

		}
		if(num_arg == 1){
			int arg = atoi(args[1]);
			if(!arg){
				printf("smash error: fg: invalid arguments\n");
				return 1; //atoi failed
			}
			pid = FindJobPID(job_list,arg); 
			if(pid == -1){
				printf("smash error: fg: job-id %d does not exist\n",arg);
				return 1;
			}
			auto it = find_job_index(job_list,arg);
			printf("%s : %d\n",(*it).cmd,pid);
			if((*it).status == STOPPED){
				if(kill(pid,SIGCONT) == -1){
					perror("smash error: kill failed");
					return 1; //signal failed
				}

			}
			fg_job =  *it;  
			if(waitpid(pid,NULL,WUNTRACED) == -1){
				perror("smash error: waitpid failed");
				return 1;
			}
			else{
				removejob(job_list,arg);  //removejob takes job-id finds the element in vector and then deletes it
				fg_job.process_ID = -1;
				return 0;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		//remove all finished jobs
		JobDone(job_list);
		if(num_arg > 1 || num_arg < 0){
			printf("smash error: bg: invalid arguments\n");
			return 1;
		}
		//no arguments sent
		if(num_arg == 0){
			int max_stopped_id = MaxStopJobID(job_list);  //implement later, returns job-id;
			if(max_stopped_id == -1){
				printf("smash error: bg: there is stopped jobs to resume\n");
				return 1;
			}
			pid = FindJobPID(job_list,max_stopped_id);
			auto it = find_job_index(job_list,max_stopped_id);
			printf("%s : %d\n", (*it).cmd,pid);
			if(kill(pid,SIGCONT)== -1){
				perror("smash error: kill failed");
				return 1;
			}
			else{
				(*it).status = BG;
				return 0;
			}
				
		}
		if(num_arg ==1){
			int arg1 = atoi(args[1]);
			if(!arg1){
				return 1;
			}
			pid = FindJobPID(job_list,arg1);
			if(pid == -1){
				printf("smash error: bg: job-id %d does not exist\n",arg1);
				return 1;
			}
			auto it = find_job_index(job_list,arg1);
			if((*it).status == BG){
				printf("smash error: bg: job-id %d is already running in the background\n",arg1);
				return 1;
			}
			printf("%s : %d\n",(*it).cmd,pid);
			if(kill(pid,SIGCONT)== -1){
				perror("smash error: kill failed");
				return 1;
			}
			else{
				(*it).status = BG;
				return 0;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
			//remove all finished jobs
			JobDone(job_list);
			if(!strcmp(arg[1],"kill")){
				int size = job_list.size();
				for(int i=0; i<size;i++){
					auto it = job_list.begin();
					char* cmd = (*it).cmd;
					int job_id = (*it).job_id;
					pid = (*it).process_ID;
					if(kill(pid,SIGTERM)== -1){
						perror("smash error: kill failed");
						return 1;
					}
					printf("[%d]%s-Sending SIGTERM... ",job_id,cmd);
					//check if process is alive and well
					int state = kill(pid,0);
					if(state == -1){
						perror("smash error: kill failed");
						return 1;
					}else if(state == 0){
						sleep(5);
						printf("(5 sec passed)Sending SIGKILL... ");
						if(kill(pid,SIGKILL) == -1 ){
							perror("smash error: kill failed");
							return 1;
						}
						removejob(job_list,job_id);
						printf("Done.\n");
						return 0;
					}else{
						removejob(job_list,job_id);
						printf("Done.\n");
						return 0;
					}
					
				}
			}
			else{
				exit(0);
			}
			
			
			
	}
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
 		if(num_arg != 2){
			printf("smash error: diff: invalid arguments\n");
		}
		FILE* fp1 = fopen(arg[1],"r");
		FILE* fp2 = fopen(arg[2],"r");
		
		if (fp1 == NULL || fp2 == NULL) {
			//printf("Failed to open one of the files.\n");
			return 1;
		}
		
		int ch1, ch2;
		int diff_flag = 0;

		do {
			ch1 = fgetc(file1);
			ch2 = fgetc(file2);

			if (ch1 != ch2) {
				diff_flag = 1;
				break;
			}
		} while (ch1 != EOF && ch2 != EOF);
		
		fclose(fp1);
		fclose(fp2);

		if (diff_flag) {
			printf("1\n");
			return 1;
		} else if (ch1 == EOF && ch2 == EOF) {     
			printf("0\n");
			return 0;
		}else{
			printf("1\n");
			return 1;
		} 
	}	
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	/*if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}*/
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					perror("smash error: fork failed");
					exit(1);
					break;
        	case 0 :
                	// Child Process
               		setpgrp();
					if(execvp(args[0],args)== -1){
						perror("smash error: execvp failed");
						exit(1);
						break;
					}
			
			default:
					//excute father process
					bool bg_flag = false;
					//checking if cmd needs to excute in background
					if(!strcmp(args[num_arg],"&") && num_arg > 0){
						bg_flag = true;
						
					}
					if(bg_flag){
						int index = job_list.back();
						int job_id = (job_list[index].job_id)+1;
						jobs new_job = jobs(NULL,pID,job_id,BG,args[0]);//create new job
						job_list.insert(job_list.end(), new_job);//add job to list
						exit(0);
						break;
					}
					else{
						//process runs in fg
						int status;
						int waitpid_ret_val = waitpid(pID,&status,WUNTRACED);
						if(waitpid_ret_val == -1){
							perror("smash error: waitpid failed");
							exit(1);
							break;
						}
						if(waitpid_ret_val == pID && WIFSTOPPED(status)){
							int index = job_list.back();
							int job_id = (job_list[index].job_id)+1;
							jobs new_job = jobs(NULL,pID,job_id,STOPPED,args[0]);//create new job
							job_list.insert(job_list.end(), new_job);//add job to list
							exit(0);
							break;
						}
						if(waitpid_ret_val == pID && WIFEXITED(status)){
							fg_job.process_ID = -1;
							exit(0);
							break;
						}
					}
					
					
					
	}
	exit(0);
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)      //not needed!!!!!!
{

	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
		
		
		
					
		/* 
		your code
		*/
		
	}
	return -1;
}

