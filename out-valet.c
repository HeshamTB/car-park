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
#include "CarPark.h"
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
    //++id;
    Car* car = NULL;
    
    while (true) {
        /*
            Wait on parked sem. indicates there is cars. Otherwise sleep on it
        */
        setVoState(id, READY);
        //sem_wait(&lock_parked); // Wait until there are parked cars
        car = NULL;
        setVoState(id, WAIT);
        /* Safely read parked cars */
        pthread_mutex_lock(&writer);
        /* Look for car that is due to leave (linear search) */
        for (int i = 0; i < psize; i++) {
            time_t now = time(NULL);
            if (car_parks[i] == NULL) continue;
            if (!(car_parks[i]->ptm + car_parks[i]->ltm < now)) continue; // To prevent null access segv
            /* Car i is due to leave */
            setVoState(id, MOVE);
            car = car_parks[i];      /* Take a refrence */
            car_parks[i] = NULL;     /* Clear parking space */
            oc--;                    /* Safe within writer lock context CS */
            setVoCar(id, car);
            usleep((int)(((double)rand() /RAND_MAX)*pow(10,6)));
            break;
        }
        pthread_mutex_unlock(&writer);

        /* Record parked time */
        if (car == NULL) continue;
        sem_post(&empty); /* Signal an empty slot */ // TODO if we make this point to an index we can save time
        //setVoCar(id, car);
        time_t delta_park_time = time(NULL) - car->ptm;
        sem_wait(&spt_mutex);
        spt += delta_park_time;
        sem_post(&spt_mutex);
        free(car);
    }

    pthread_exit(0);
}