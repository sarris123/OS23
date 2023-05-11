// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include <string.h>

extern jobs fg_job;
extern vector<jobs> job_list; 

void ctrl_c_handler (int signum){
	
	if(signum != SIGKILL || fg_job.process_ID == -1){
		return;
	}
	printf("smash: caught ctrl-C\n");
	if(kill(fg_job.process_ID,SIGKILL) == -1){
		perror("smash error: kill failed");
		return;
	}
	printf("smash: process %d was kill\n",fg_pid);
	//remove job from foreground
	fg_job.process_ID = -1;
	return;
	
}

void ctrl_z_handler(int signum){
	if(signum != SIGSTOP || fg_job.process_ID == -1 ){
		return;
	}
	printf("smash: caught ctrl-Z\n");
	if(kill(fg_job.process_ID,SIGSTOP) == -1){
		perror("smash error: kill failed");
		return;
	}
	printf("smash: process %d was stopped\n",fg_pid);
	
	//add stopped job to job list
	int index = job_list.size()-1;
	int job_id = (job_list[index].job_id)+1;
	jobs stopped_job = jobs(NULL,fg_pid,job_id,STOPPED,fg_job.cmd);//create stopped job
	job_list.insert(job_list.end(), stopped_job);
	
	//remove job from foreground
	fg_job.process_ID = -1;

	return;
	
	
}


string get_sig_name(int signum){
	switch(signum){
	case 1: return "SIGHUP";
		break;
	case 2: return "SIGINT";
		break;
	case 3: return "SIGQUIT";
		break;
	case 4: return "SIGILL";
		break;
	case 5: return "SIGTRACE";
		break;
	case 6: return "SIGABORT";
		break;
	case 7: return "SIGBUS";
		break;
	case 8: return "SIGFLOAT";
		break;
	case 9: return "SIGKILL";
		break;
	case 10: return "SIGUSER1";
		break;
	case 11: return "SIGSEGFAULT";
		break;
	case 12: return "SIGUSER2";
		break;
	case 13: return "SIGBROKENPIPE";
		break;
	case 14: return "SIGALARMCLK";
		break;
	case 15: return "SIGTERM";
		break;
	case 16: return "SIGSTACK";
		break;
	case 17: return "SIGCHILD";
		break;
	case 18: return "SIGCONT";
		break;
	case 19: return "SIGSTOP";
		break;
	case 20: return "SIGSTP";
		break;
	case 21: return "SIGSTOPTTYIN";
		break;
	case 22: return "SIGSTOPTTYOUT";
		break;
	case 23: return "SIGURGENTIO";
		break;
	case 24: return "SIGCPUTIME";
		break;
	case 25: return "SIGFILESIZE";
		break;
	case 26: return "SIGVIRTIMER";
		break;
	case 27: return "SIGPROFTIMER";
		break;
	case 28: return "SIGWINDOW";
		break;
	case 29: return "SIGIOPOSIBLE";
		break;
	case 30: return "SIGPOWER";
		break;
	case 31: return "SIGBADSYSCALL";
		break;
	default: return "";
	}
	return NULL;
}


