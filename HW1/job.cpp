
#include "job.h"
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>

using namespace std; //not sure

vector<jobs> job_list;

void JobDone (vector<jobs> job_list){
	
	for(auto it = job_list.begin(); it != job_list.end();){
		if(waitpid((*it).process_ID,NULL,WNOHANG) != 0){
			job_list.erase(it);
		}else{
			++it;
		}
	}
	
}

//helper functions
int FindJobPID(vector<jobs> job_list,int job_id){
	for(auto it = job_list.begin(); it != job_list.end();){
		if((*it).job_id == job_id){
			return (*it).process_ID;
		}else{
			++it;
		}
	}
	return -1;
}


int MaxStopJobID(vector<jobs> job_list){
	if(job_list.size() == 0){
		//list is empty
		return -1;
	}
	for(auto it = job_list.end(); it != job_list.begin();){
		if((*it).status == STOPPED){
			return (*it).job_id;
		}else{
			--it;
		}
	}
	return -1;
}


vector<jobs>::iterator find_job_index(vector<jobs> job_list,int job_id){
	for(auto it = job_list.begin(); it != job_list.end();){
		if((*it).job_id == job_id){
			return it;
		}else{
			++it;
		}
	}
	return job_list.end();
}

void removejob(vector<jobs> job_list,int job_id){
		for(auto it = job_list.begin(); it != job_list.end();){
			if((*it).job_id == job_id){
				job_list.erase(it);
				break;
			}
		}
}



