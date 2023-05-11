#include <stdio.h>
#include <time.h>
#define DEFULT_ID 0


typedef enum status {STOPPED = 0 , BG = 1};



class jobs{
	public:
	double time;
	int process_ID;
	int job_id;
	status status;
	char* cmd;
	
	#defult constructor
	jobs(): time(time(NULL)),process_ID(DEFULT_ID), job_id(0),status(BG),cmd(){};   //add cmd 
	
	
	#constructor
	jobs(double time,int process_ID,int job_id,int status,char* cmd) : time(time(NULL)),
																		process_ID(process_ID),
																		job_id(job_id),
																		status(status),
																		cmd(cmd) {};
															
	#copy constructor
	jobs(const jobs &j){
		time = j.time;
		process_ID = j.process_ID;
		job_id = j.job_id;
		status = j.status;
		strcpy(cmd,j.cmd);
	}
	
	#operator&=
	jobs& operator=(const jobs& j){
		if(this != &j){
			time = j.time;
			process_ID = j.process_ID;
			job_id = j.job_id;
			status = j.status;
			strcpy(cmd,j.cmd);
		}
		return *this;
	}
	
	#destructor
	~jobs();
	
};


//helper function
vector<jobs>::iterator find_job_index(vector<jobs> job_list,int job_id);

MaxStopJobID(vector<jobs> job_list);

FindJobPID(vector<jobs> job_list,int job_id);

JobDone (vector<jobs> job_list);

removejob(vector<jobs> job_list,int job_id);
	
	
	