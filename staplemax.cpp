#include <iostream>
#include <map>
#include <pthread.h>
#include <fstream>


using namespace std;

const int PENS = 200, PAPER = 100, TONER = 35, LAPTOPS = 20;
const int SUCCESS = 0;
const int NUM_THREADS = 20;
const string FILEPATH = "/home/fac/lillethd/cpsc3500/projects/p3a";
const string TESTPATHWIN = "C:\\Users\\Nolan\\CLionProjects\\CPSC3500HW3\\sales";
const string TESTPATHMAC = "/Users/smithn10/Documents/CPSC3500-HW3/CPSC3500HW3/sales";


struct prodSum{
    int sum;
};
map<string, int> prodMap;
pthread_mutex_t mutlock;

void *processLine(void *threadNumber) {
    pthread_mutex_lock(&mutlock);

    ifstream infile;
    string fileName, prod;

    int sum = 0;

    long fileNumber = (long) threadNumber;
    //development filepath
    fileName = TESTPATHMAC + to_string(fileNumber + 1) + ".txt";
    //deployment filepath
    //fileName = FILEPATH + to_string(fileNumber + 1) + ".txt";
    infile.open(fileName);

    if (infile.fail()) {
        cout << "ERROR: Unable to read file... exiting thread" << endl;
        pthread_exit(NULL);
    }

    while (infile >> prod) {
        if (prod == "pen") {
            prodMap.at(prod) -= 1;
            if (prodMap.at(prod) == 0) {
                prodMap.at(prod) = PENS;
            }
        } else if (prod == "paper") {
            prodMap.at(prod) -= 1;
            if (prodMap.at(prod) == 0) {
                prodMap.at(prod) = PAPER;
            }
        } else if (prod == "toner") {
            prodMap.at(prod) -= 1;
            if (prodMap.at(prod) == 0) {
                prodMap.at(prod) = TONER;
            }
        } else if (prod == "laptop") {
            prodMap.at(prod) -= 1;
            if (prodMap.at(prod) == 0) {
                prodMap.at(prod) = LAPTOPS;
            }
        }
        sum++;
    }
    struct prodSum* threadProdSum = NULL;
    threadProdSum= new prodSum;

    threadProdSum->sum+=sum;
    infile.close();
    prodMap.at("sum") += sum;
    pthread_mutex_unlock(&mutlock);

    return (void* ) threadProdSum;

}


int main(void) {


    prodMap["pen"] = 200;
    prodMap["paper"] = 100;
    prodMap["toner"] = 35;
    prodMap["laptop"] = 20;
    prodMap["sum"] = 0;

    struct prodSum* prodSum1 = NULL;
    prodSum1= new struct prodSum;

    prodSum1->sum=0;
    if (pthread_mutex_init(&mutlock, NULL) != 0) {
        cout << "mutex init failed" << endl;
        return 1;
    }

    pthread_t threadID[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threadID[i], NULL, &processLine, (void *) i) != SUCCESS) {
            cout << "ERROR: failed to create thread " << i << " " << endl;
            return 0;
        }
    }

    struct prodSum *tempSum;
    struct prodSum **getTempSum = &tempSum;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threadID[i], (void**)getTempSum) != SUCCESS) {
            cout << "Error: failed to join thread " << i << endl;
            return 0;
        }
        pthread_mutex_lock(&mutlock);
        prodSum1->sum += tempSum->sum;
        pthread_mutex_unlock(&mutlock);

    }

    cout << "Pens: " << prodMap.at("pen") << endl;
    cout << "Paper: " << prodMap.at("paper") << endl;
    cout << "Toner: " << prodMap.at("toner") << endl;
    cout << "Laptops: " << prodMap.at("laptop") << endl;
    cout << "Total sales: " << prodSum1->sum << endl;
    pthread_mutex_destroy(&mutlock);


    return 0;
}