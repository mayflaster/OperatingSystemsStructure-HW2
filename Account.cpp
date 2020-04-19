//
// Created by mayfl on 10/12/2019.
//

#include "Account.h"


Account:: Account(int accountNumber, int accPassword, int initBalance):accountNumber(accountNumber),
        password(accPassword),balance(initBalance){
    readerCounter = 0;
    pthread_mutex_init(&readLock, NULL);
    pthread_mutex_init(&writeLock,NULL);
}
Account::Account() {
    accountNumber = 0;
    password=0;
    balance=0;
    readerCounter = 0;
    pthread_mutex_init(&readLock, NULL);
    pthread_mutex_init(&writeLock,NULL);
}

Account:: ~Account(){
    pthread_mutex_destroy(&readLock);
    pthread_mutex_destroy(&writeLock);
}


int Account:: getAccountId(){
    return accountNumber;
}
int Account:: getPassword(){
    return password;
}
int Account::getBalance(){
    return balance;
}


void Account:: depositAccount (int amount){
    balance+=amount;
}
void Account:: withdrawAccount(int amount){
    balance -=  amount;
}
void Account:: lockRead(){
    pthread_mutex_lock(&readLock);
    readerCounter++;
    if(readerCounter == 1){
        lockWrite();
    }
    pthread_mutex_unlock(&readLock);

}
void Account:: lockWrite(){
    pthread_mutex_lock(&writeLock);
}
void Account::unlockRead(){
    pthread_mutex_lock(&readLock);
    readerCounter--;
    if(readerCounter == 0){
        unlockWrite();
    }
    pthread_mutex_unlock(&readLock);
}


void Account::unlockWrite(){
    pthread_mutex_unlock(&writeLock);

}