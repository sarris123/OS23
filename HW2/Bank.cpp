
#include "Bank.h"



#define COMISSION_SLEEP 3
#define MIN_PERCENTAGE 1
#define MAX_PERCENTAGE 5
#define PRINT_SLEEP 500000

extern Log_File* out_log;
extern Bank bank;
extern vector<ATM> ATM_vec;

void add_account_to_Bank(account new_account, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    curr_account.push_back(new_account);
    sort(curr_account.begin(), curr_account.end(), compare_accountID);
    bank.set_Bank_accounts(curr_account);

}

bool check_password(int account_ID, int Password, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    int index = check_account_index(account_ID, bank);
    if(curr_account[index].get_Password() == Password){
        return true;
    }
    return false;
}

int check_account_index(int account_ID, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    for(unsigned int i = 0; i < curr_account.size(); ++i){
        if(curr_account[i].get_acc_ID() == account_ID){
            return i;
        }
    }
    return -1;
}

int deposit_money(Bank bank, int account_ID, int amount){
    vector<account> curr_account = bank.get_Bank_accounts();
    int index = check_account_index(account_ID, bank);
    curr_account[index].account_enter_writers();
    int new_amount = curr_account[index].get_Balance() + amount;
    curr_account[index].set_Balance(new_amount);
    bank.set_Bank_accounts(curr_account);
    curr_account[index].account_leave_writers();
    return new_amount;
}

bool check_balance_enough(int account_ID, int amount, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    int index = check_account_index(account_ID, bank);
    if(curr_account[index].get_Balance() < amount){
        return false;
    }
    return true;
}

int withdraw_money(int account_ID, int amount, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    int index = check_account_index(account_ID, bank);
    curr_account[index].account_enter_writers();
    int new_amount = curr_account[index].get_Balance() - amount;
    curr_account[index].set_Balance(new_amount);
    bank.set_Bank_accounts(curr_account);
    curr_account[index].account_leave_writers();
    return new_amount;
}

void delete_account(int account_ID, Bank bank){
    vector<account> curr_account = bank.get_Bank_accounts();
    for(vector<account>::iterator it = curr_account.begin(); it != curr_account.end();){
		if(it->get_acc_ID() == account_ID){
			curr_account.erase(it);
			break;
		}
		else {
			++it;
		}
	}
    bank.set_Bank_accounts(curr_account);
}




void* Bank_comission_handler(void* bank_isOpen){
	printf("in commission\n");
    bool status = (bool) bank_isOpen;
    double commission_percentage = 0.0, charge_amount = 0.0;
    while(status){
        sleep(COMISSION_SLEEP);
        //lock the accounts
        bank.bank_enter_readers();
        //randomize a number between 1%-5%
        commission_percentage = (rand() % (MIN_PERCENTAGE + 1)) + (double)(MAX_PERCENTAGE - MIN_PERCENTAGE);
        vector<account> current_vec = bank.get_Bank_accounts();
        int bank_gain = 0;
        for(unsigned int i = 0; i < current_vec.size(); ++i){
            current_vec[i].account_enter_writers();
            int prev_balance = current_vec[i].get_Balance();
            charge_amount = round(commission_percentage*0.01* current_vec[i].get_Balance());
            bank_gain += charge_amount;
            current_vec[i].set_Balance(prev_balance-charge_amount);
            out_log -> lock_log_file();
            out_log -> logFile << "Bank: commissions of " << commission_percentage 
                     << " % were charged, the bank gained " << charge_amount 
                     << " $ from account " << current_vec[i].get_acc_ID() << endl; 
            out_log -> unlock_log_file();
            current_vec[i].account_leave_writers();
        }
        bank.set_Bank_accounts(current_vec);
        int current_bank_balance = bank.get_Bank_Balance();
        bank.set_Bank_Balance(current_bank_balance + bank_gain);
        bank.bank_leave_readers();
    }
    pthread_exit(NULL);
}

void* Bank_printing_handler(void* bank_isOpen){
    bool status = (bool) bank_isOpen;
    while(status){
        usleep(PRINT_SLEEP);
        print_left_top_corner();
        cout << "Current Bank Status" << endl;
        bank.bank_enter_readers();
		printf("printingg\n");
        vector<account> current_vec = bank.get_Bank_accounts();
        for(unsigned int i = 0; i < current_vec.size(); ++i){
            current_vec[i].account_enter_readers();
            cout << "Account " << current_vec[i].get_acc_ID() 
                 << ": Balance - " << current_vec[i].get_Balance() 
                 << " $, Account Password - " << current_vec[i].get_Password() << endl; 
            current_vec[i].account_leave_readers();
        }
        cout << "The Bank has " << bank.get_Bank_Balance() << " $" << endl;
        bank.bank_leave_readers();
    }
    pthread_exit(NULL);
}


void print_left_top_corner(){
	printf("\033[2J");
	printf("\033[1;1H");
}