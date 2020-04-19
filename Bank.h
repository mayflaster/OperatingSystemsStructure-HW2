//
// Created by mayfl on 10/12/2019.
//

#ifndef WET2_BANK_H
#define WET2_BANK_H



#include "Account.h"



class Bank{
    Account bankAccount;

public:
    bool finishFlag;
    /*
     * initialize new bank with private account
     */
    Bank();
    ~Bank();
    /**
     * every 3 seconds this function prints the status of all the accounts in the system
     * (namely all the accounts in the global vector)
     * it prints the accounts sorted by account number and prints their balance and password
     *  it also prints the current bank account status
     */
    void printCurrentAccountsStatus();
    /**
     * every 0.5 seconds this function calculate a percent between 2 to 4 and take commission
     * from all the accounts in the system, and adds it to the bank private account
     */
    void takeCommissions();

};

#endif //WET2_BANK_H
