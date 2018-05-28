#include <iostream>
#include <map>
#include <pthread.h>
#include <fstream>


using namespace std;

const int PENS = 200,PAPER = 100,TONER=35,LAPTOPS=20;
const int SUCCESS = 0;
const int NUM_THREADS = 20;
const string FILEPATH = "/home/fac/lillethd/cpsc3500/projects/p3a";
const string TESTPATH = "C:\\Users\\Nolan\\CLionProjects\\CPSC3500HW3\\sales";

struct products {
    int pen;
    int paper;
    int toner;
    int laptops;
};


map<string, int> prodMap;


void *processLine(void *threadNumber) {
    ifstream infile;
    string fileName, prod;

    int penCount = 0;
    int paperCount = 0;
    int tonerCount = 0;
    int laptopCount = 0;

    long fileNumber = (long) threadNumber;
    //development filepath
    cout << TESTPATH + to_string(fileNumber + 1) + ".txt"<< endl;
    fileName = TESTPATH + to_string(fileNumber + 1) + ".txt";
    //deployment filepath
    //fileName = FILEPATH + to_string(fileNumber + 1) + ".txt";
    infile.open(fileName);

    if (infile.fail()) {
        cout << "ERROR: Unable to read file... exiting thread" << endl;
        pthread_exit(NULL);
    }

    while (infile >> prod) {
        if(prod=="pen"){
            penCount++;
        }
        else if(prod=="laptop"){
            laptopCount++;
        }
        else if(prod=="toner"){
            tonerCount++;
        }
        else if(prod=="paper"){
            paperCount++;
        }
    }
    infile.close();

    struct products *retProd = NULL;
    retProd = new products;
    retProd->pen -= penCount;
    retProd->paper -= paperCount;
    retProd->toner -= tonerCount;
    retProd->laptops -= laptopCount;

    return (void*) retProd;

}




int main(void) {

    struct products *p = NULL;
    p = new struct products;

    prodMap["pen"] = 200;
    prodMap["paper"] = 100;
    prodMap["toner"] = 35;
    prodMap["laptop"] = 20;
    p->pen = 200;
    p->paper = 100;
    p->toner = 35;
    p->laptops = 20;

    pthread_t threadID[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threadID[i], NULL, processLine, (void *) i) != SUCCESS) {
            cout << "ERROR: failed to create thread " << i << " " << endl;
            return 0;
        }
    }

    struct products *tempP;
    struct products **getTempP = &tempP;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threadID[i], (void **) getTempP) != SUCCESS) {
            cout << "Error: failed to join thread " << i << endl;
            return 0;
        }

        p->pen = tempP->pen;
        p->paper = tempP->paper;
        p->toner = tempP->toner;
        p->laptops = tempP->laptops;
    }

    cout << "Pens: " << p->pen << endl;
    cout << "Paper: " << p->paper << endl;
    cout << "Toner: " << p->toner << endl;
    cout << "Laptops: " << p->laptops << endl;

    delete p;
    delete tempP;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}