#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "CPSimulator.h"
#include "CarPark.h"
#include "out-valet.h"


sem_t lock_check;  //can we move it to init in CPSimulator.c ? 
atomic_int turn;

int num_out_valets = 0;
pthread_t *tid_out = NULL;


///**
    Initialize out-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Hesham T. Banafa 
*/
int init_out_valets(int number_valets)
{
    num_out_valets = number_valets;
    tid_out = calloc(number_valets,sizeof(pthread_t));
    
    pthread_t out_valet_threads[number_valets];
    sem_init(&lock_check, 0, 1);                //we may need to move it to init in CPSimulator.c
    turn = 0;
    for (int i = 0; i < number_valets; i++) {
        pthread_create(&out_valet_threads[i],
                       NULL,
                       run_out_valets, 
                       (void *)(int64_t)i);
    }
    return 0;
}

///** 
    Entry point for out-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Hesham T. Banafa
*/
void *run_out_valets(void *args)
{
    int id = (int64_t)args;
    Car* car = NULL;
    
    while (true) {

        setVoState(id, READY);
        sem_wait(&lock_check);
        setVoState(id, WAIT);

        /* A solution to prevent starvation and operate in a RR */
        if (turn != id) {
            /* Not our turn */
            sem_post(&lock_check); continue; /* Release lock and go back */
        }
        turn++;
        if (turn == out_valets) turn = 0; /* Reset to first thread */
        
        while (car == NULL) {
            /* Aquire car_park lock */
            pthread_mutex_lock(&writer);
            /* Look for car that is due to leave (linear search) */
            for (int i = 0; i < psize; i++) {
                if (car_parks[i] == NULL) continue; /* Empty slot */
                if (!(car_parks[i]->ltm + car_parks[i]->ptm < time(NULL))) continue;
                setVoState(id, MOVE);
                car = car_parks[i];      /* Take a refrence */
                car_parks[i] = NULL;     /* Clear parking space */
                oc--;                    /* Safe within writer lock context CS */
                setVoCar(id, car);
                sem_post(&empty); /* Signal an empty slot */ // TODO if we make this point to an index we can save time
                usleep((int)(((double)rand() /RAND_MAX)*pow(10,6)));
                break;
            }
            pthread_mutex_unlock(&writer);
        }
        sem_post(&lock_check);

        /* Record parked time */
        time_t delta_park_time = time(NULL) - car->ptm;
        sem_wait(&spt_mutex);
        spt += delta_park_time;
        sem_post(&spt_mutex);
        free(car);
        car = NULL;
    }

    pthread_exit(0);
}


void term_outvalets(){
     for (int i=0; i<num_out_valets; i++){
          pthread_cancel(tid_out[i]);
     }
}