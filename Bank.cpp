//
// Created by mayfl on 10/12/2019.
//

#include "Bank.h"
#include <algorithm>
using namespace std;

vector<Account> accounts;
pthread_mutex_t printLock;
ofstream outputFile;
//ofstream testfile;
//ofstream testfile2;
pthread_mutex_t readLockBank;
pthread_mutex_t writeLockBank;
int BankReadCounter=0;


/**
 * this function is called before "write" operation of the Bank (  opening account)
 * lock the bank writer mutex - means that the bank system  (and  ATMs )is locked
 */
void lockWriteBank(){
    pthread_mutex_lock(&writeLockBank);
}
/*
 *  unlock the bank writer mutex - means that the bank system is unlocked
 */
void unlockWriteBank(){
    pthread_mutex_unlock(&writeLockBank);

}

/**
 * this function is called before "read" operations of the Bank ( all the operation except opening account)
 * lock the bank reader mutex -
 *  update num of "readers" safely
 */
void  lockReadBank(){
    pthread_mutex_lock(&readLockBank);
    BankReadCounter++;
    if(BankReadCounter == 1){
        lockWriteBank();
    }
    pthread_mutex_unlock(&readLockBank);

}
/**
 * unlock the bank read lock - to update the number of "readers" safely.
 * called after "readers" operations
 */
void unlockReadBank(){
    pthread_mutex_lock(&readLockBank);
    BankReadCounter--;
    if(BankReadCounter == 0){
        unlockWriteBank();
    }
    pthread_mutex_unlock(&readLockBank);
}


Bank ::Bank():finishFlag(false) {
    pthread_mutex_init(&readLockBank, NULL);
    pthread_mutex_init(&writeLockBank,NULL);
}

Bank::~Bank() {
    pthread_mutex_destroy(&readLockBank);
    pthread_mutex_destroy(&writeLockBank);
}

/* Returns a random double between min and max */
/*double get_random(double min, double max) {
    double randomCom = (max-min) * ((double) rand() / (double) RAND_MAX) + min;
    return randomCom;
}*/

int randomNumber (){
    int commission = ((rand() % 3) + 2);
    return commission;
}

/*
void Bank::takeCommissions(){
    while(finishFlag){

    }
}*/

/*void Bank::takeCommissions(){
   while (1){
        sleep(3);
       lockReadBank();
       //  pthread_mutex_lock(&printLock);
        double commission = get_random(2,4);
        vector<Account>::iterator it;
        for(it=accounts.begin(); it<accounts.end(); it++){
            it->lockRead();
            double balance =(double) it->getBalance();
            int amountToTransfer = (int) (balance*commission)/100;
            it->unlockRead();
            bankAccount.lockWrite();
            it->lockWrite();
            it->withdrawAccount(amountToTransfer);
            bankAccount.depositAccount(amountToTransfer);
            it->unlockWrite();
            bankAccount.unlockWrite();
            pthread_mutex_lock(&printLock);
            outputFile << "Bank: commissions of " <<  commission <<" % were charged, the bank gained " << amountToTransfer <<
            " $ from account " << it->getAccountId() <<endl;
            pthread_mutex_unlock(&printLock);
        }

       unlockReadBank();
      // pthread_mutex_unlock(&printLock);
       //
    }
}*/


void Bank::takeCommissions(){
   // testfile2.open("test2.txt");
   while (!finishFlag){
        sleep(3);
     //   testfile2 << "start round!" << endl;
       //testfile << "start round!" << endl;

       lockReadBank();
       //testfile2 << "start round adter lock!" << endl;
       //testfile << "start round adfter lock!" << endl;

       //  pthread_mutex_lock(&printLock);
        //double commission = get_random(2,4);
        int commission =randomNumber();
        vector<int> commissions;
        int sumForBank=0;
        vector<Account>::iterator it;
        for(it=accounts.begin(); it<accounts.end(); ++it){
            //it->lockRead();
         //   testfile2 << "lock account " << it->getAccountId() << endl;
            it->lockWrite();
           // testfile2 << "after lock account " << it->getAccountId() << endl;
            int balance = it->getBalance();
            int amountToTransfer = (int) (balance*commission)/100;
            commissions.push_back(amountToTransfer);
            it->withdrawAccount(amountToTransfer);
          //  testfile2 << "unlock account " << it->getAccountId() << endl;
            it->unlockWrite();
            sumForBank+=amountToTransfer;
           // it->unlockRead();
        }
    //   testfile2 << "lock bank " << endl;
        bankAccount.lockWrite();
        bankAccount.depositAccount(sumForBank);
      // testfile2 << "unlock bank " << endl;
        bankAccount.unlockWrite();
        vector<int>::iterator it2=commissions.begin();
        pthread_mutex_lock(&printLock);
        for(it=accounts.begin(); it<accounts.end(); ++it){
            outputFile << "Bank: commissions of " <<  commission <<" % were charged, the bank gained " << *it2 <<
            " $ from account " << it->getAccountId() <<endl;
            ++it2;
        }
        pthread_mutex_unlock(&printLock);
       //testfile << "unlock10" << endl;
       unlockReadBank();
      // pthread_mutex_unlock(&printLock);
       //
   }
   //testfile2.close();
   //pthread_exit(NULL);
}


/**
 * compare 2 accounts by their account numner.
 * return true if a's account number is smaller tht b and false otherwise
 */
bool compareAccounts(Account a, Account b){
    return (a.getAccountId() < b.getAccountId());
}



void Bank::printCurrentAccountsStatus(){
    while(!finishFlag){
        usleep(500000);
        lockWriteBank();
        sort(accounts.begin(),accounts.end(),compareAccounts);
        unlockWriteBank();
        lockReadBank();
        cout << "\033[2J\033[1;1H";
        cout <<"Current Bank Status" << endl;
        vector<Account>::iterator it;
        for(it=accounts.begin();it<accounts.end();++it){
            it->lockRead();
            cout << "Account " <<  it->getAccountId() << ": Balance - " <<it->getBalance() << " $ , Account Password - ";
                 int pass = it->getPassword();
                 if (pass<10)cout << "000";
                 else if(pass < 100) cout << "00";
                 else if(pass<1000) cout << "0";
                 cout << pass<< endl;
            it->unlockRead();
        }
        bankAccount.lockRead();
        cout << "The Bank has " << bankAccount.getBalance() << " $" << endl;
        bankAccount.unlockRead();
        unlockReadBank();
    }

}




