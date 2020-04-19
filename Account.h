//
// Created by mayfl on 10/12/2019.
//

#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Account{
    int accountNumber;
    int password;
    int balance;
    int readerCounter;
    pthread_mutex_t readLock;
    pthread_mutex_t writeLock;

public:
    /**
     * initialize new account with given parameters
     * @param accountNumber - id of the account
     * @param accPassword  - password of the account
     * @param initBalance - initial balance of the new account
     */
    Account(int accountNumber, int accPassword, int initBalance);
    /**
     *  initialize new account with default parameters
     */
    Account();
    ~Account();
    int getAccountId();
    /**
    * get information about the current balance of the account
    */
    int getPassword();
    /**
     * get information about the current balance of the account
     */
    int getBalance(); //read
    /*
     *adding money (amount) to the balance of the account
     */
    void depositAccount (int amount); // write
    /**
     * decrease money (amount) from the balance of the account
     */
    void withdrawAccount(int amount); // write
    /*
    *lock the accounts from reading operations (get balance).
    */
    void lockRead();
    /*
    * lock the accounts to writing operations (deposit,withdraw and transfer).
    */
    void lockWrite();
    /*
     * unlock the accounts to writing operations (get balance)
    */
    void unlockRead();
    /*
     * unlock the accounts to writing operations (deposit,withdraw and transfer)
     */
    void unlockWrite();

};



#endif //WET2_ACCOUNT_H
