#ifndef BANK_H_
#define BANK_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdbool.h>
#include "account.h"
#include "ATM.h"
#include "Log-File.h"

using namespace std;


class Bank{
private:
    vector<account> Bank_accounts;
    int Bank_Balance;
    int readers_count;
    pthread_mutex_t bank_read_lock;
    pthread_mutex_t bank_write_lock;

public:
    //default construcor
    Bank(){
		printf("in bank con\n");
        Bank_accounts = vector<account>();
        Bank_Balance = 0;
        readers_count = 0;
        bank_init(&bank_read_lock);
        bank_init(&bank_write_lock);
		printf("finshed bank\n");
    }

    //constructor
    Bank(vector<account>Bank_accounts, int Bank_Balance, int readers_count, pthread_mutex_t bank_read_lock, pthread_mutex_t bank_write_lock){
        Bank_accounts = Bank_accounts;
        Bank_Balance = Bank_Balance;
        readers_count = 0;
        bank_init(&bank_read_lock);
        bank_init(&bank_write_lock);
    }

    //copy constructor
    /*Bank(Bank const &B){
        bank_init(bank_read_lock);
        bank_init(bank_write_lock);
        bank_mutex_lock(B.bank_read_lock);
        bank_mutex_lock(B.bank_write_lock);
        Bank_accounts = B.Bank_accounts;
        Bank_Balance = B.Bank_Balance;
        readers_count = B.readers_count;
        bank_mutex_unlock(B.bank_read_lock);
        bank_mutex_unlock(B.bank_write_lock);
    }*/


    //destructor
    ~Bank(){
        if(pthread_mutex_destroy(&bank_read_lock)){
			printf("whhhhh\n");
            perror("Bank error: pthread_mutex_destroy failed");
            exit(1);
        }
        if(pthread_mutex_destroy(&bank_write_lock)){
			printf("whhhhhyy\n");
            perror("Bank error: pthread_mutex_destroy failed");
            exit(1);
        }
    }

    //operator "="
    /*Bank& operator=(const Bank& b){
		if(this != &b){
			printf("opertato = \n");
            //bank_mutex_lock(bank_read_lock);
            bank_mutex_lock(bank_write_lock);
			printf("lock issue = \n");
            bank_mutex_lock(b.bank_read_lock);
            bank_mutex_lock(b.bank_write_lock);  
			Bank_accounts = b.Bank_accounts;
            Bank_Balance = b.Bank_Balance;
            readers_count = b.readers_count;
            bank_mutex_unlock(b.bank_write_lock);
			printf("unlock issue = \n");
			bank_mutex_unlock(b.bank_read_lock);
            
            //bank_mutex_unlock(bank_read_lock);
            bank_mutex_unlock(bank_write_lock);
			printf("opertato = finished\n");
		}
		return *this;
	}*/

    //get functions
    std::vector<account>get_Bank_accounts(){
        return Bank_accounts;
    }

    int get_Bank_Balance(){
        return Bank_Balance;
    }

    //set functions
    void set_Bank_accounts(const vector<account>& accounts){
        Bank_accounts = accounts;
    }

    void set_Bank_Balance(int balance){
        Bank_Balance = balance;
    }

    void bank_init(pthread_mutex_t* a){
        if(pthread_mutex_init(a, NULL)){
            perror("Bank error: pthread_mutex_init failed");
            exit(1);
        }
		printf("done mutex_init\n");
		
    }

    //mutex lock function
    void bank_mutex_lock(pthread_mutex_t* m){
		printf("gonna start locking\n");
		int result=pthread_mutex_lock(m);
		printf("sys call res is %d\n",result);
        if(result){
			printf("did I eneter by mistake\n");
            perror("Bank error: pthread_mutex_lock failed");
            exit(1);
        }
		printf("done locking\n");

    }
    //mutex unlock function
    void bank_mutex_unlock(pthread_mutex_t* m){
        if(pthread_mutex_unlock(m)){
            perror("Bank error: pthread_mutex_unlock failed");
            exit(1);
        }

    }
    //reader lock function
    void bank_enter_readers(){
        bank_mutex_lock(&bank_read_lock);
        readers_count++;
        if(readers_count == 1){
			printf("readers count = %d\n", readers_count);
            bank_mutex_lock(&bank_write_lock);
			printf("unlockk1\n");
        }
        bank_mutex_unlock(&bank_read_lock);
		printf("unlockkk\n");
    }

    //reader unlock function
    void bank_leave_readers(){
        bank_mutex_lock(&bank_read_lock);
        readers_count--;
        if(readers_count == 0){
            bank_mutex_unlock(&bank_write_lock);
        }
        bank_mutex_unlock(&bank_read_lock);
    }

    //writer unlock function 
    void bank_leave_writers(){
        bank_mutex_unlock(&bank_write_lock);
    }

    //writer lock function
    void bank_enter_writers(){
        bank_mutex_lock(&bank_write_lock);
    }
};


//function declaration
void add_account_to_Bank(account new_account, Bank bank);
bool check_password(int account_ID, int Password, Bank bank);
int check_account_index(int account_ID, Bank bank);
int deposit_money(Bank bank, int account_ID, int amount);
bool check_balance_enough(int account_ID, int amount, Bank bank);
int withdraw_money(int account_ID, int amount, Bank bank);
void delete_account(int account_ID, Bank bank);
void print_left_top_corner();
void* Bank_comission_handler(void* bank_isOpen);
void* Bank_printing_handler(void* bank_isOpen);












#endif // !BANK_H