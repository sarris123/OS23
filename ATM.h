#ifndef ATM_H_
#define ATM_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <string>
#include <cstring>
#include "account.h"
#include "Log-File.h"
#include "Bank.h"

using namespace std;

class ATM{
    private:
        int ATM_num;
        string file_name;
    public:

        ATM( int ATM_num, string file_name){
            ATM_num = ATM_num;
            file_name = file_name; // check later if we need new char*
        }

        ATM(ATM const & atm){
            ATM_num = atm.ATM_num;
            file_name = atm.file_name;
        }

        ~ATM(){}

        //get functions
        int get_ATM_num(){
            return ATM_num;
        }

        string get_file_name(){
            return file_name;
        }

        //set functions
        void set_ATM_num(int num){
            ATM_num = num;
        }

        void set_file_name(string input){
            file_name = input;
        }

};

//function declaration
void* ATM_thread_handler(void* atm);
std::vector<std::string>splitString(const std::string& line);








#endif // !ATM_H