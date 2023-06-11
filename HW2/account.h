#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>




class account
{
    private:
        int acc_ID;
        int Password;
        int Balance;
        int readers_num;
        pthread_mutex_t readers;
        pthread_mutex_t writers;
    public:
    //constructor
    account(int acc_ID, int Password, int Balance, int readers_num, pthread_mutex_t readers, pthread_mutex_t writers){
        acc_ID = acc_ID;
        Password = Password;
        Balance = Balance;
        readers_num = 0;
        init(readers);
        init(writers);
    }

    //copy constructor
     account(account const &acc){
        init(readers);
        init(writers);
        mutex_lock(acc.readers);
        mutex_lock(acc.writers);
        acc_ID = acc.acc_ID;
        Password = acc.Password;
        Balance = acc.Balance;
        readers_num = acc.readers_num;
        mutex_unlock(acc.readers);
        mutex_unlock(acc.writers);      
    }


    //destructor
    ~account(){
        if(pthread_mutex_destroy(&readers)){
			printf("ha ha ha im in acc\n");
            perror("Bank error: pthread_mutex_destroy failed");
            exit(1);
        }
        if(pthread_mutex_destroy(&writers)){
			printf("ha ha ha im in acc2\n");
            perror("Bank error: pthread_mutex_destroy failed");
            exit(1);
        }
    }

    //operator "="
    account& operator=(const account& acc){
		if(this != &acc){
            mutex_lock(readers);
            mutex_lock(writers);
            mutex_lock(acc.readers);
            mutex_lock(acc.writers);  
			acc_ID = acc.acc_ID;
			Password = acc.Password;
			Balance = acc.Balance;
            readers_num = acc.readers_num;
            mutex_unlock(acc.readers);
            mutex_unlock(acc.writers);
            mutex_unlock(readers);
            mutex_unlock(writers);
		}
		return *this;
	}

    //get functions 
    int get_acc_ID() const{
        return acc_ID;
    }

    int get_Password() const{
        return Password;
    }

    int get_Balance() const{
        return Balance;
    }

    //set functions
    void set_acc_ID(int ID){
        acc_ID = ID;
    }
    void set_Password(int Pass){
        Password = Pass;
    }
    void set_Balance(int Bal){
        Balance = Bal;
    }
    //mutex init function
    void init(pthread_mutex_t a){
        if(pthread_mutex_init(&a, NULL)){
            perror("Bank error: pthread_mutex_init failed");
            exit(1);
        }
    }
    //mutex lock function
    void mutex_lock(pthread_mutex_t m){
        if(pthread_mutex_lock(&m)){
            perror("Bank error: pthread_mutex_lock failed");
            exit(1);
        }

    }
    //mutex unlock function
    void mutex_unlock(pthread_mutex_t m){
        if(pthread_mutex_unlock(&m)){
            perror("Bank error: pthread_mutex_unlock failed");
            exit(1);
        }

    }
    //reader lock function
    void account_enter_readers(){
        mutex_lock(readers);
        readers_num++;
        if(readers_num == 1){
            mutex_lock(writers);
        }
        mutex_unlock(readers);
    }

    //reader unlock function
    void account_leave_readers(){
        mutex_lock(readers);
        readers_num--;
        if(readers_num == 0){
            mutex_unlock(writers);
        }
        mutex_unlock(readers);
    }

    //writer unlock function 
    void account_leave_writers(){
        mutex_unlock(writers);
    }

    //writer lock function
    void account_enter_writers(){
        mutex_lock(writers);
    }
};

//functions declaration
bool compare_accountID(const account& j1, const account& j2);









#endif // !ACCOUNT_H