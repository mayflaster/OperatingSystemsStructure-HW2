#include <iostream>
#include "ATM.h"
#include "Bank.h"
extern vector<Account> accounts;
extern pthread_mutex_t printLock;
extern ofstream outputFile;
//extern ofstream testfile;

vector<ATM> ATMs;

/**
 * wrapping function for executeInputFile of ATM.
 * @param arg - pointer to the ATM
 */
void* InputFileWrap(void * arg){
    ((ATM*)arg)->executeInputFile();
    pthread_exit(NULL);

}
/**
 * wrapping function for printCurrentAccountsStatus of Bank.
 * @param arg - pointer to the Bank
 */
void * printCurrentAccountsStatusWrap(void* arg){
    ((Bank*) arg)->printCurrentAccountsStatus();
    pthread_exit(NULL);
}
/**
 * wrapping function for takeCommissionsWrap of Bank.
 * @param arg - pointer to the Bank
 */
void * takeCommissionsWrap(void* arg){
    ((Bank*) arg)->takeCommissions();
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc < 3){
        cerr << "illegal arguments" << endl;
        exit(1);
    }

    int numOfATMs= atoi(argv[1]);
    if (numOfATMs == 0){
        cerr << "illegal input" << endl;
        exit(1);
    }

    if(argc-2 != numOfATMs) {
        cerr << "illegal input" << endl;
        exit(1);
    }
    pthread_mutex_init(&printLock, NULL);
    outputFile.open("log.txt");

   // testfile.open("test.txt");
    for (int i = 0; i < numOfATMs; ++i) {
        char* fileString = argv[i+2];
        ifstream file(fileString); //input file stream

        //string line;
        if (!file) {
            // File doesn't exist or some other error
           cerr << "illegal arguments" << endl;
            exit(1);
        }
        //file.open("fileString");
        const ATM a(i+1,fileString);
        ATMs.push_back(a);
    }

    Bank bank;
    //create threads
    //ATM threads
    pthread_t * threads = new pthread_t[numOfATMs];
    for (int j = 0; j < numOfATMs; ++j) {
        try{
            pthread_create(&threads[j],NULL, InputFileWrap, (void*)&ATMs[j]);
        }
        catch (...){
            cerr << "error in thread creating"<<endl;
            exit (-1);
        }

    }
    //bank threads
    pthread_t bankThread[2];
    try{
        pthread_create(&bankThread[0],NULL,printCurrentAccountsStatusWrap, (void*)&bank);
    }
    catch(...){
        cerr << "error in thread creating"<<endl;
        exit (-1);
    }
    try{
        pthread_create(&bankThread[1],NULL,takeCommissionsWrap, (void*)&bank);
    }
    catch (...){
        cerr << "error in thread creating"<<endl;
        exit (-1);
    }



    // join threads
    for (int k = 0; k < numOfATMs; ++k) {
        try{
            pthread_join(threads[k],NULL);
        }
        catch (...){
            cerr << "thread join error" << endl;
            exit (-1);
        }

    }
    //after finish all thread = >
     bank.finishFlag = true;
    try{
        pthread_join(bankThread[0],NULL);
    }
    catch (...){
        cerr << "thread join error" << endl;
        exit (-1);
    }

    try{
        pthread_join(bankThread[1],NULL);
    }
    catch (...){
        cerr << "thread join error" << endl;
        exit (-1);
    }


    //destroy accounts vector

    delete [] threads;
    pthread_mutex_destroy(&printLock);
    outputFile.close();
    return 0;

}

