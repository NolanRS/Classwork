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
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + (N-1)) % N
#define RIGHT (phnum + 1) % N


sem_t locker;
sem_t *forks;
int *state;
int N;

void canEat(int phnum) {
  if (state[phnum] == HUNGRY
      && state[LEFT] != EATING
      && state[RIGHT] != EATING) {
    // state that eating
    state[phnum] = EATING;

    //    sleep(2);
    printf("Philosopher %d is Eating\n", phnum + 1);

    // sem_post(&S[phnum]) has no effect
    // during takefork
    // used to wake up hungry philosophers
    // during putfork
    mdat_sem_post(&forks[phnum]);
  }
}

void takeFork(int phnum) {

  mdat_sem_wait(&locker);

  // state that hungry
  state[phnum] = HUNGRY;

  printf("Philosopher %d is Hungry\n", phnum + 1);

  // eat if neighbours are not eating
  canEat(phnum);

  mdat_sem_post(&locker);

  // if unable to eat wait to be signalled
  mdat_sem_wait(&forks[phnum]);
  //  sleep(1);
}

void putFork(int phnum) {

  mdat_sem_wait(&locker);

  // state that thinking
  state[phnum] = THINKING;

  printf("Philosopher %d is thinking\n", phnum + 1);

  canEat(LEFT);
  canEat(RIGHT);

  mdat_sem_post(&locker);
}

void
sectionInitGlobals(int numPhilosophers) {
  int i = 0;
  char istring[5];
  forks = malloc(numPhilosophers * sizeof(sem_t));
  state = malloc(numPhilosophers * sizeof(sem_t));
  mdat_sem_init("startphSem", &locker, 0, 1);

  for (i = 0; i < numPhilosophers; i++) {
    sprintf(istring,"%d",i);
    mdat_sem_init(istring, &forks[i], 0, 0);
  }
  N = numPhilosophers;
}

void sectionRunPhilosopher(int philosopherID, int numRounds) {
  int i;
  for(i=0;i<numRounds;i++){
    //   sleep(1);
    takeFork(philosopherID);
    eat();
    //   sleep(0);
    putFork(philosopherID);
    think();
  }


}



