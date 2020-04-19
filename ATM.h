//
// Created by mayfl on 10/12/2019.
//

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>



using namespace std;
//ofstream *outputFile;

class ATM{
    int numberATM;
    string inputFile;
public:
    /**
     * initialize new ATM with given parameters
     * @param serialNum - id of the account
     * @param inputFile  - input files of operations for the ATM
     */
    ATM(int serialNum,string inputFile);
    /**
     * initialize new ATM with default parameters
     */
    ATM();
    /**
     * main function of all the ATM in the vector.
     * each ATM performs the file's operations.
     * the function identify the operation and continues to the proper treatment function
     */
    void  executeInputFile();
    /**
     * opens new account and adds it to the accounts vector.
     * when an ATM opens account, the Bank and the other ATMs are suspended
     * @param accountNum - id of the account
     * @param password  -password for the new account
     * @param initBalance - initial balance of the new account
     */
    int openAccount(int accountNum, int password, int initBalance);
    /**
     * search account in the accounts vector and
     * deposit amount of money (if founded)
     * @param accountNum  - number of the account
     * @param password  - password of the account - required for this operation
     * @param amount = amount to deposit in the account
     */
    int depositAccount (int accountNum, int password, int amount);
    /**
     * search account in the accounts vector and
     * withdraw amount of money (if founded)
     * @param accountNum  - number of the account
     * @param password  - password of the account - required for this operation
     * @param amount - amount to withdraw from the account
     */
    int withdrawAccount (int accountNum, int password, int amount); // write
    /**
     * search account in the accounts vector and return its balance of money (if founded)
     * @param accountNum -number of the account
     * @param password -  password of the account - required for this operation
     * @return - balance of the account
     */
    int getBalance (int accountNum, int password); //read
    /**
     * search both accounts (target and source) in the accounts vector and transfer money from source to target.
     * in case that source does not have enough money, an error is printed to output file
     * @param sourceAccount - account to take money from
     * @param password - of the source account
     * @param targetAccount - the account that gets the money (taken from source)
     * @param amount - amount to transfer between the accounts
     */
    int transferAccounts (int sourceAccount, int password, int targetAccount, int amount);

};



#endif //WET2_ATM_H
