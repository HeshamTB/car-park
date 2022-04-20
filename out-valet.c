


#include <semaphore.h>
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
#include "out-valet.h"

/**
    Initialize out-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Hesham T. Banafa 
*/
int init_out_valets(int number_valets)
{
    pthread_t out_valet_threads[number_valets];

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
    ++id;

    while (true) {
        /*
            Wait on parked sem. indicates there is cars. Otherwise sleep on it
        */
        sem_wait(&lock_parked); // Wait until there are parked cars
        Car* car;
        /* Safely read parked cars */
        pthread_mutex_lock(&writer);
        /* Look for car that is due to leave (linear search) */
        for (int i = 0; i < psize; i++) {
            if (car_parks[i] == NULL) continue;
            if (!(car_parks[i]->ltm < time(NULL))) continue; // To prevent null access segv
            /* Car i is due to leave */
            car = car_parks[i];      /* Take a refrence */
            car_parks[i] = NULL;     /* Clear parking space */
            oc--;                    /* Safe within writer lock context CS */
            usleep((int)(((double)rand() /RAND_MAX)*pow(10,6)));
            break;
        }
        pthread_mutex_unlock(&writer);
        sem_post(&empty); /* Signal an empty slot */ // TODO if we make this point to an index we can save time
        
        /* Record parked time */
        time_t delta_park_time = time(NULL) - car->ptm;
        sem_wait(&spt_mutex);
        spt += delta_park_time;
        sem_post(&spt_mutex);
        free(car);
    }

    pthread_exit(0);
}