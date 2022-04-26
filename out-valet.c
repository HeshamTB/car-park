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


sem_t lock_check;
atomic_int turn;

/**
    Initialize out-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Hesham T. Banafa 
*/
int init_out_valets(int number_valets)
{
    pthread_t out_valet_threads[number_valets];
    sem_init(&lock_check, 0, 1);
    turn = 0;
    for (int i = 0; i < number_valets; i++) {
        pthread_create(&out_valet_threads[i],
                       NULL,
                       run_out_valets, 
                       (void *)(int64_t)i);
    }
    return 0;
}

/** 
    Entry point for out-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Hesham T. Banafa
*/
void *run_out_valets(void *args)
{
    int id = (int64_t)args;
    //++id;
    Car* car = NULL;
    
    while (true) {
        /*
            Wait on parked sem. indicates there is cars. Otherwise sleep on it
        */
        setVoState(id, READY);
        //sem_wait(&lock_parked); // Wait until there are parked cars

        sem_wait(&lock_check);
        setVoState(id, WAIT);
        /* A solution to prevent starvation and operate in a RR */
        if (turn != id) {
            /* Not our turn */
            sem_post(&lock_check); continue; /* Release lock and go back */
        }
        turn++;
        if (turn == out_valets) turn = 0; /* Reset to first thread */
        printf("out-valet %d checking\n", id);
        
        /* Safely read parked cars */
        /* Look for car that is due to leave (linear search) */
        int j = 0;
        while (car == NULL) {
            /* Aquire car_park lock */
            pthread_mutex_lock(&writer);
            for (int i = 0; i < psize; i++) {
                if (car_parks[i] == NULL) continue; /* Empty slot */
                if (!(car_parks[i]->ltm + car_parks[i]->ptm < time(NULL))) continue;
                setVoState(id, MOVE);
                car = car_parks[i];      /* Take a refrence */
                car_parks[i] = NULL;     /* Clear parking space */
                oc--;                    /* Safe within writer lock context CS */
                setVoCar(id, car);
                usleep((int)(((double)rand() /RAND_MAX)*pow(10,6)));
                break;
            }
            pthread_mutex_unlock(&writer);
        }
        sem_post(&lock_check);

        /* Record parked time */

        sem_post(&empty); /* Signal an empty slot */ // TODO if we make this point to an index we can save time
        //setVoCar(id, car);
        time_t delta_park_time = time(NULL) - car->ptm;
        sem_wait(&spt_mutex);
        spt += delta_park_time;
        sem_post(&spt_mutex);
        free(car);
        car = NULL;
    }

    pthread_exit(0);
}