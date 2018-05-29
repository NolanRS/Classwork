//  sections1.c: mutual exclusion model sections

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "sections.h"
#include "mdat.h"


#define LEFT (philosopherID + (N-1)) % N
#define RIGHT (philosopherID + 1) % N
pthread_mutex_t *forks;
int N;

void sectionInitGlobals(int numPhilosophers) {
    int i = 0;
    char istring[5];
    forks = malloc(numPhilosophers * sizeof(pthread_mutex_t));
    for (i = 0; i < numPhilosophers; i++) {
        sprintf(istring, "%d", i);
        mdat_mutex_init(istring, &forks[i], NULL);
    }
    N = numPhilosophers;
}

void sectionRunPhilosopher(int philosopherID, int numRounds) {
    int i;
    for (i = 0; i < numRounds; i++) {
        mdat_mutex_lock(&forks[LEFT]);
        mdat_mutex_lock(&forks[RIGHT]);
        printf("philosopher %d is eating\n", philosopherID + 1);
        eat();
        printf("philosopher %d is thinking\n", philosopherID + 1);
        think();
        mdat_mutex_unlock(&forks[LEFT]);
        mdat_mutex_unlock(&forks[RIGHT]);
    }


}



