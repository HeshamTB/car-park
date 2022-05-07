#ifndef CPSIM_H
#define CPSIM_H


#include <semaphore.h>
#include <signal.h>

#ifndef CAR_H
#include "Car.h"
#endif

extern int oc, nm, psize, out_valets;
extern long nc, pk, rf, sqw, spt;
extern sem_t mutex, arrivals, sqw_mutex, in_held_mutex, empty, lock_parked, spt_mutex;
extern pthread_mutex_t writer;
extern Car** car_parks;

/**
    intializes the semaphores, the car park array, the car queue and the GUI
    @author Muhannad Al-Ghamdi
    @date Apr 18th, 2022
*/
void init();

/**
    cleaning up and printing the stats
    @author Muhannad Al-Ghamdi
    @date Apr 25th, 2022
*/
void cleanup();
/**
    Print help in case of incorrect usage
    @author Hesham T. Banafa
    @date Apr 16th, 2022
*/
void usage();

/**
    Read in args from argv
    @author Hesham T. Banafa
    @date Apr 17th, 2022
*/
void process_args(char *argv[], int argc, int *in_val, int *out_val, int *qsize, double *exp_cars);

/**
    Signal Handler for interuption
    @author Hesham T. Banafa
    @date Apr 16th, 2022
*/
void sigint_handler();
/**
    Signal Handler for termination
    @author Hesham T. Banafa
    @date Apr 16th, 2022
*/
void sigterm_handler();

#endif