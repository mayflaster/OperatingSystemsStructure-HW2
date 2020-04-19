 //
// Created by mayfl on 10/12/2019.
//

#include "ATM.h"
#include "Account.h"

 extern vector<Account> accounts;
 extern pthread_mutex_t printLock;
 extern ofstream outputFile;
 extern void lockWriteBank();
 extern void unlockWriteBank();
 extern void  lockReadBank();
extern  void unlockReadBank();
//extern ofstream testfile;


int findIndex(int accountNum){
    vector<Account>::iterator it;
    int index = 0;
    for (it = accounts.begin(); it <accounts.end(); ++it){
        if (it->getAccountId() == accountNum){
            return index;
        }
        index++;
    }
    return -1;
}
 ATM::ATM(int serialNum,string inputFile): numberATM(serialNum), inputFile(inputFile){
 }
 ATM::ATM(){
      numberATM =0 ;
     inputFile='\0';
}

void ATM:: executeInputFile(){
    string line;
    ifstream cmd(inputFile.c_str());
    char operation;
    int account, password,amount,targetAccount;
    while(getline(cmd,line)) {
        stringstream ss(line);
        ss >> operation;
        switch (operation){
            case 'O':{
                ss>> account >> password >>amount ;
               // testfile<< numberATM<<": lock 1 account " << account <<endl;
               lockWriteBank();
                //testfile<< numberATM<<": after lock 1 account " << account <<endl;
                openAccount(account,password,amount);
                unlockWriteBank();
                //testfile<< numberATM<<": unlock 1 account " << account <<endl;
                break;
            }
            case 'D':{

                lockReadBank();

                ss>>account>>password>>amount;
                //testfile<< numberATM<<": lock D account " << account <<endl;
                depositAccount(account,password, amount);
                unlockReadBank();
                break;
            }
            case 'W':{
                lockReadBank();

                ss>>account>>password>>amount;
                //testfile<< numberATM<<": lock W account " << account <<endl;
                withdrawAccount(account, password, amount);
                unlockReadBank();
                break;
            }
            case 'B':{
                lockReadBank();

                ss>>account>>password;
                //testfile<< numberATM<<": lock B account " << account <<endl;
                getBalance(account,password);
                unlockReadBank();
                break;
            }
            case 'T':{
                lockReadBank();

                ss>>account>>password>>amount>>targetAccount;
                //testfile<< numberATM<<": lock T account " << account << "to " << targetAccount << endl;
                transferAccounts(account, password,targetAccount, amount);
                unlockReadBank();
                break;
            }
        }
        usleep(100000);
    }
    //pthread_exit(NULL);
}







int ATM:: getBalance (int accountNum, int password){
    int index = findIndex(accountNum);
    //pthread_mutex_lock(&printLock);
    if (index <0){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – account id " << accountNum << " does not exist" <<endl;
        sleep(1);
        pthread_mutex_unlock(&printLock);
        return 0;
    }
    if (accounts[index].getPassword() != password){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – password for account id " << accountNum << " is incorrect" <<endl;
       sleep(1);
        pthread_mutex_unlock(&printLock);
        return 0;
    }
    accounts[index].lockRead();
    pthread_mutex_lock(&printLock);
    outputFile << numberATM << ": account " << accountNum << " balance is " << accounts[index].getBalance()<< endl;
    sleep(1);
    pthread_mutex_unlock(&printLock);
    accounts[index].unlockRead();
    return 0;
}

int ATM:: transferAccounts (int sourceAccount, int password, int targetAccount, int amount){
    //pthread_mutex_lock(&printLock);
    if(sourceAccount == targetAccount){
        return 0;
    }
    int indexSource = findIndex(sourceAccount);
    int indexTarget = findIndex(targetAccount);
    if (indexSource <0){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – account id " << sourceAccount << " does not exist" <<endl;
        sleep(1);
        pthread_mutex_unlock(&printLock);
        return 0;
    }
    if (indexTarget <0){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – account id " << targetAccount << " does not exist" <<endl;
        sleep(1);
        pthread_mutex_unlock(&printLock);
        return 0;
    }
    if (accounts[indexSource].getPassword()!=password){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – password for account id " << sourceAccount << " is incorrect" <<endl;
        sleep(1);
        pthread_mutex_unlock(&printLock);
        return 0;
    }
    //accounts[indexSource].lockRead();
    accounts[min(indexSource,indexTarget)].lockWrite();
    accounts[max(indexSource,indexTarget)].lockWrite();
    int sourceBalance=accounts[indexSource].getBalance();
    if (sourceBalance < amount){
        pthread_mutex_lock(&printLock);
        outputFile << "Error " << numberATM << ": Your transaction failed – account id " << sourceAccount << " balance is lower than " << amount <<endl;
        sleep(1);
        pthread_mutex_unlock(&printLock);
        accounts[max(indexSource,indexTarget)].unlockWrite();
        accounts[min(indexSource,indexTarget)].unlockWrite();
        //accounts[indexSource].unlockRead();
        return 0;
    }

    //accounts[indexSource].unlockRead();
    //accounts[min(indexSource,indexTarget)].lockWrite();
    //accounts[max(indexSource,indexTarget)].lockWrite();
    accounts[indexSource].withdrawAccount(amount);
    accounts[indexTarget].depositAccount(amount);
    sourceBalance=accounts[indexSource].getBalance();
    int targetBalance=accounts[indexTarget].getBalance();
    //accounts[indexSource].lockRead();
    //accounts[indexSource].lockRead();

    pthread_mutex_lock(&printLock);
    outputFile << numberATM << ": Transfer " << amount << " from account " << sourceAccount<<  " to account "  << targetAccount <<  "  new account balance is "
    << sourceBalance << " new target account balance is " <<targetBalance << endl;
    sleep(1);
    pthread_mutex_unlock(&printLock);
    accounts[max(indexSource,indexTarget)].unlockWrite();
    accounts[min(indexSource,indexTarget)].unlockWrite();
    return 0;
}


 int  ATM:: openAccount(int accountNum, int password, int initBalance){
     //pthread_mutex_lock(&printLock);
     int index = findIndex(accountNum);
     if (index >=0){  // Account already exist
         pthread_mutex_lock(&printLock);
         outputFile << "Error " << this->numberATM  << ": Your transaction failed – account with the same id exists"<<endl;
        sleep(1);
         pthread_mutex_unlock(&printLock);
         return -1;
     }
     //Account *newAccount= new Account(accountNum,password,initBalance);.
     pthread_mutex_lock(&printLock);
      Account newAccount(accountNum,password,initBalance);
     accounts.push_back(newAccount);

     outputFile << this->numberATM << ": New account id is "  << accountNum << " with password "<< password <<" and initial balance "<< initBalance<< endl;
     sleep(1);
     pthread_mutex_unlock(&printLock);
     return 0;
 }

 int ATM::depositAccount (int accountNum, int password, int amount){
     //pthread_mutex_lock(&printLock);
     int index = findIndex(accountNum);
     if (index <0){
         pthread_mutex_lock(&printLock);
         outputFile << "Error " << this->numberATM  << ": Your transaction failed – account id "<< accountNum <<" does not exist"<<endl;
         sleep(1);
         pthread_mutex_unlock(&printLock);
         return -1;
     }
     if (accounts[index].getPassword() == password){
         accounts[index].lockWrite();
         accounts[index].depositAccount(amount);
         //accounts[index].unlockWrite();
        // accounts[index].lockRead();
         pthread_mutex_lock(&printLock);
         outputFile <<  this->numberATM  << ": Account "<< accountNum<< " new balance is " <<accounts[index].getBalance()<<" after "<< amount << " $ was deposited"<<endl;
         sleep(1);
         pthread_mutex_unlock(&printLock);
         accounts[index].unlockWrite();
         //accounts[index].unlockRead();

         return 0;
     }
     else {
         pthread_mutex_lock(&printLock);
         outputFile << "Error " << this->numberATM  << ": Your transaction failed – password for account id "<< accountNum<< " is incorrect" <<endl;
         sleep(1);
         pthread_mutex_unlock(&printLock);
         return -2;
     }
     return -5;
 }


 int ATM::withdrawAccount (int accountNum, int password, int amount){
     //pthread_mutex_lock(&printLock);
     int index = findIndex(accountNum);
     if (index <0){
         pthread_mutex_lock(&printLock);
         outputFile << "Error " << this->numberATM  << ": Your transaction failed – account id "<< accountNum <<" does not exist"<<endl;
         sleep(1);
         pthread_mutex_unlock(&printLock);
         return -1;
     }
     if (accounts[index].getPassword() == password){
         //accounts[index].lockRead();
         accounts[index].lockWrite();
         if(accounts[index].getBalance()>=amount){
             //accounts[index].unlockRead();
             //accounts[index].lockWrite();
             accounts[index].withdrawAccount(amount);
             //accounts[index].unlockWrite();
             //accounts[index].lockRead();
             pthread_mutex_lock(&printLock);
             outputFile <<  this->numberATM  << ": Account "<< accountNum<< " new balance is " <<accounts[index].getBalance()<<" after "<< amount << " $ was withdrew"<<endl;
             sleep(1);
             pthread_mutex_unlock(&printLock);
             //accounts[index].unlockRead();
             accounts[index].unlockWrite();
             return 0;
         }
         else {
            // accounts[index].unlockRead();
             pthread_mutex_lock(&printLock);
             outputFile << "Error " << this->numberATM  << ": Your transaction failed – account id "<< accountNum <<" balance is lower than "<<amount<<endl;
             sleep(1);
             pthread_mutex_unlock(&printLock);
             accounts[index].unlockWrite();
             return -3;
         }

     }
     else {
         pthread_mutex_lock(&printLock);
         outputFile << "Error " << this->numberATM  << ": Your transaction failed – password for account id "<< accountNum<< "is incorrect" <<endl;
         sleep(1);
         pthread_mutex_unlock(&printLock);
         return -2;
     }
     return -5;

 } // write