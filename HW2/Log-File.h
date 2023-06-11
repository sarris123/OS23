#ifndef _LOG_FILE_
#define _LOG_FILE_

#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;

class Log_File{
    pthread_mutex_t log_lock;

public:
    ofstream logFile;
    Log_File() {
		printf("in log\n");
        logFile.open("log.txt");
		printf("done open log\n");
        if (pthread_mutex_init(&log_lock, NULL)) {
			perror("Bank error: pthread_mutex_init failed");
			exit(1);
        }
    }

    ~Log_File() {
        logFile.close();
        if (pthread_mutex_destroy(&log_lock)) {
			perror("Bank error: pthread_mutex_destroy failed");
			exit(1);
        }
    }

    void lock_log_file() {
        if (pthread_mutex_lock(&log_lock)) {
			perror("Bank error: pthread_mutex_lock failed");
			exit(1);
        }
    }

    void unlock_log_file() {
        if (pthread_mutex_unlock(&log_lock)) {
			perror("Bank error: pthread_mutex_unlock failed");
			exit(1);
        }
    }
};







#endif  //!_LOG_FILE_