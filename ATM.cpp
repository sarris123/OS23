#include <string>
#include "ATM.h"
#include <cstdlib>

#define OPERATION_DELAY 1


extern Bank bank;
extern Log_File* out_log;
using namespace std;

void* ATM_thread_handler(void* atm){
    ifstream inputFile;
    ATM* ATM_P = (ATM*)atm; //if problem try static casting
    string file_name =  ATM_P -> get_file_name();
    int ATM_num = ATM_P -> get_ATM_num();
    inputFile.open(file_name.c_str());
    if(!inputFile){
        cerr << "Bank error: illegal arguments" << endl;
        exit(1);
    }
    string line;
    while(!inputFile.eof()){
        getline(inputFile, line);
        vector<std::string> operation_arguments = splitString(line);
        string ATM_operation = operation_arguments[0];
        char ATM_op = *ATM_operation.c_str();
        int account_ID = 0;
        int Password = 0;
        int amount = 0;
        
        if(ATM_op == 'O'){
            if(operation_arguments.size() != 4){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_writers();
            sleep(OPERATION_DELAY);
			const char* acc_str = operation_arguments[1].c_str(); 
            account_ID = atoi(acc_str);
			const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
			const char* amount_str = operation_arguments[3].c_str();
            int initial_amount = atoi(amount_str);
            //check if the account already exist
            if(check_account_index(account_ID, bank) != -1){ 
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num <<": Your transaction failed - account with the same id exists" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            else {
               
               pthread_mutex_t read_m;
			   pthread_mutex_t write_m;
               account new_account = account(account_ID,Password, initial_amount, 0 , read_m,  write_m);
               add_account_to_Bank(new_account, bank);  
               out_log -> lock_log_file();
               out_log -> logFile << ATM_num << ": New account id is " << account_ID 
                     << " with password " << Password << " and initial balance " << initial_amount << endl; 
               out_log -> unlock_log_file();
            }
            bank.bank_leave_writers();
            break;
        }
            
        else if(ATM_op == 'D'){
             if(operation_arguments.size() != 4){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_readers();
            sleep(OPERATION_DELAY);
            const char* acc_str = operation_arguments[1].c_str(); 
            account_ID = atoi(acc_str);
            const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
            const char* amount_str = operation_arguments[3].c_str();
            amount = atoi(amount_str);
            //check account if exists
            if(check_account_index(account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            //check if password correct
            if(!check_password(account_ID, Password, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - password for account id " 
                    << account_ID << " is incorrect" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            int withdraw_new_balance = deposit_money(bank, account_ID, amount);
            out_log -> lock_log_file();
            out_log -> logFile << ATM_num << ": Account " << account_ID 
                << " new balance is " << withdraw_new_balance <<" after " << amount << " $ was deposited" << endl;
            out_log -> unlock_log_file();
            bank.bank_leave_readers();
        }
        
           

        else if(ATM_op == 'W'){
            if(operation_arguments.size() != 4){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_readers();
            sleep(OPERATION_DELAY);
            const char* acc_str = operation_arguments[1].c_str(); 
            account_ID = atoi(acc_str);
            const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
            const char* amount_str = operation_arguments[3].c_str();
            amount = atoi(amount_str);
            //check account if exists
            if(check_account_index(account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            //check if password correct
            if(!check_password(account_ID, Password, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - password for account id " 
                    << account_ID << " is incorrect" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            //check if the balance is enough
            if(!check_balance_enough(account_ID, amount, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << account_ID << " balance is lower than " << amount << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            int new_balance = withdraw_money(account_ID, amount, bank);
            out_log -> lock_log_file();
            out_log -> logFile << ATM_num << ": Account " << account_ID 
                << " new balance is " << new_balance <<" after " << amount << " $ was withdrew" << endl;
            out_log -> unlock_log_file();
            bank.bank_leave_readers();
        }
            
            

        else if(ATM_op == 'B'){
            if(operation_arguments.size() != 3){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_readers();
            sleep(OPERATION_DELAY);
            const char* acc_str = operation_arguments[1].c_str();
            account_ID = atoi(acc_str);
            const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
             if(check_account_index(account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            if(!check_password(account_ID, Password, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - password for account id " 
                    << account_ID << " is incorrect" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            amount = withdraw_money(account_ID, 0, bank);
            out_log -> lock_log_file();
            out_log -> logFile << ATM_num << ": Account " << account_ID 
                << " balance is " << amount << endl;
            out_log -> unlock_log_file();
            bank.bank_leave_readers();
            
        }
            

        else if(ATM_op == 'Q'){
            if(operation_arguments.size() != 3){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_writers();
            sleep(OPERATION_DELAY);
            const char* acc_str = operation_arguments[1].c_str();
            account_ID = atoi(acc_str);
            const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
            if(check_account_index(account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            if(!check_password(account_ID, Password, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - password for account id " 
                    << account_ID << " is incorrect" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            delete_account(account_ID, bank);
            amount = withdraw_money(account_ID, 0, bank); 
            out_log -> lock_log_file();
            out_log -> logFile << ATM_num << ": Account " << account_ID 
                << " is now closed. Balance was " << amount << endl;
            out_log -> unlock_log_file();
            bank.bank_leave_writers();
        }
            
        else if(ATM_op == 'T'){
            if(operation_arguments.size() != 5){
                cerr << "Bank error: illegal arguments" << endl;
                exit(1);
            }
            bank.bank_enter_readers();
            sleep(OPERATION_DELAY);
            const char* source_str = operation_arguments[1].c_str();
            int source_account_ID = atoi(source_str);
            const char* pass_str = operation_arguments[2].c_str();
            Password = atoi(pass_str);
            const char* target_str = operation_arguments[3].c_str();
            int target_account_ID = atoi(target_str);
            const char* transfer_amount_str = operation_arguments[4].c_str();
            int transfer_amount = atoi(transfer_amount_str);
            if(check_account_index(source_account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << source_account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            if(check_account_index(target_account_ID, bank) == -1){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << target_account_ID << " does not exist" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            if(!check_password(source_account_ID, Password, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - password for account id " 
                    << source_account_ID << " is incorrect" << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            if(!check_balance_enough(source_account_ID, transfer_amount, bank)){
                out_log -> lock_log_file();
                out_log -> logFile << "Error " << ATM_num << ": Your transaction failed - account id " 
                    << source_account_ID << " balance is lower than " << transfer_amount << endl;
                out_log -> unlock_log_file();
                exit(1);
            }
            int source_balance = withdraw_money(source_account_ID, transfer_amount, bank);
            int target_balance = deposit_money(bank, target_account_ID, transfer_amount);
            out_log -> lock_log_file();
            out_log -> logFile << ATM_num << ": Transfer " << transfer_amount 
                << " from account " << source_account_ID << " to account " 
                << target_account_ID << " new account balance is "<< source_balance 
                << " new target account balance is "<< target_balance << endl;
            out_log -> unlock_log_file();
            bank.bank_leave_readers();
        }
            

        else{
            cerr << "Bank error: illegal arguments" << endl;
            exit(1);
        }
    }
    pthread_exit(NULL);
}


std::vector<std::string>splitString(const std::string& line){
    vector<string> arguments;
    istringstream iss(line);
    string argument;
    while(iss >> argument){
        arguments.push_back(argument);
    }
    return arguments;
}