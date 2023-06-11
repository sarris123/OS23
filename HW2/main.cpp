#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include "account.h"
#include "ATM.h"
#include "Bank.h"

using namespace std;

//globals 
Bank bank;
Log_File* out_log;
//vector<ATM> ATM_vec;
pthread_t* threads_arr;
pthread_t comission_thread;
pthread_t printing_thread;
bool bank_isOpen;
int main(int argc, char const *argv[]){
    //check arguments
	printf("IN MAIN\n");
    if((argc <= 1)){
         cerr << "Bank error: illegal arguments" << endl;
         return -1;
    }
	
    bank = Bank();
    out_log = new Log_File();
	printf("Done log\n");
	bank_isOpen = true;
    int num_of_ATM = argc - 1;
	vector<ATM> ATM_vec(num_of_ATM);
	printf(" numof atms is %d\n",num_of_ATM);
    threads_arr = new pthread_t[num_of_ATM];
	printf("Done arr\n");

    //creating ATM threads :)
    for(int i = 0; i < num_of_ATM; i++){
		printf("in looop\n");
        ATM_vec[i].set_ATM_num(i+1);
		printf("Done seting \n");
		string ATM_file = std::string(argv[i+1]);
        ATM_vec[i].set_file_name(ATM_file);
        if(pthread_create(&threads_arr[i], NULL, &ATM_thread_handler, (void*)&ATM_vec[i])){
            perror("Bank error: pthread_create failed");
            if (threads_arr) delete[] threads_arr;
            exit(1);
        }
    }
    
    //creating the bank comission thread
	printf("in line 53 \n");
    if(pthread_create(&comission_thread, NULL, &Bank_comission_handler,(void*)&bank_isOpen)){  //we should pass the argument Bank_comission_handler is expecting 
            perror("Bank error: pthread_create failed");
            exit(1);
    }

    //creating the printing thread
    if(pthread_create(&printing_thread, NULL, &Bank_printing_handler,(void*)&bank_isOpen)){  //we should pass the argument Bank_printing_handler is expecting
            perror("Bank error: pthread_create failed");
            exit(1);
    }
    
	//making sure all threads wait for each other
	for(int i=0;i < num_of_ATM; i++){
		if(pthread_join(threads_arr[i], NULL)){
            perror("Bank error: pthread_join failed");
            if (threads_arr) delete[] threads_arr;
             exit(1);
        }
	}


	bank_isOpen = false;

    if(pthread_join(comission_thread, NULL)){
        perror("Bank error: pthread_join failed");
        exit(1);
    }

    if(pthread_join(printing_thread, NULL)){
        perror("Bank error: pthread_join failed");
        exit(1);
    }
	delete[] threads_arr;
    delete out_log;
	return 0;
	//printf("code done\n");
	
}
