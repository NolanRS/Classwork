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


// TODO: Declare shared variables here
pthread_mutex_t left_fork;
pthread_mutex_t right_fork;


void
sectionInitGlobals(int numPhilosophers) {
    mdat_mutex_init("leftFork",&left_fork,NULL);
    mdat_mutex_init("rightFork",&right_fork,NULL);
}

void sectionRunPhilosopher(int philosopherID, int numRounds) {
    int i;
    for(i=0;i<numRounds;i++){
        mdat_mutex_lock(&left_fork);
        mdat_mutex_lock(&right_fork);
        eat();
        mdat_mutex_unlock(&left_fork);
        mdat_mutex_unlock(&right_fork);
        think();
    }


}



