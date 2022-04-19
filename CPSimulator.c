#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "CarPark.h"
#include "in-valet.h"
#include "monitor.h"
#include "string.h"

int oc, nm, psize, in_valets, out_valets, qsize;
long nc, pk, rf, sqw, spt;
Car* car_parks = NULL;

pthread_mutex_t mutex;
sem_t arrivals, sqw_mutex, in_held_mutex, empty, writer, parked, spt_mutex;

pthread_t monitor;



/**
    intializes the semaphores, the car park array, the car queue and the GUI
    @author Muhannad Al-Ghamdi
    @date Apr 18th, 2022
*/
void init(){
    //init the car Queue
    Qinit(qsize);

    
    //init semaphores
    sem_init(&arrivals, 0, qsize);
    sem_init(&sqw_mutex, 0, 1);
    sem_init(&in_held_mutex, 0, 1);
    sem_init(&empty, 0, psize);
    sem_init(&writer, 0, 1);
    sem_init(&parked, 0, 0);
    sem_init(&spt_mutex, 0, 1);
    
    //init car park array
    car_parks = calloc(psize,sizeof(Car));
    
    //init the GUI
    G2DInit(&car_parks, psize, in_valets, out_valets, mutex);
}



/**
    Entry point of program
    Parse runtime arguments, start GUI and threads
    @author Muhannad Al-Ghamdi
    @author Hesham T. Banafa
    @date Apr 15th, 2022
*/
int main(int argc, char *argv[]) 
{
    printf("Car Park Simulator\n\
Muhannad Al-Ghamdi - Hesham T. Banafa\n");

    oc = 0;
    nm = 0;
    psize = PARK_SIZE;
    nc = 0;
    pk = 0;
    rf = 0;
    sqw = 0;
    spt = 0;

    in_valets = IN_VALETS;
    out_valets = OUT_VALETS;
    qsize = QUEUE_SIZE;
    double exp_cars = EXP_CARS;

    /* Process arguments */
    process_args(argv, argc, &in_valets, &out_valets, &qsize, &exp_cars);
    printf("%d %d %d %d %.2f\n", psize, in_valets, out_valets, qsize, exp_cars);
    /* if some of the optional args are not set to a non-zero, init with default */
 



    //[TEST]: test the GUI   
    init();
    newCars(1);
    //initalize a car for testing
    Car car;
    car.cid = 1;
    car.vid = 1;
    car.sno = 0;
    char carimg[20]=".cars/Car0.bmp";
    char carimgf[20]=".cars/Car0f.bmp";

    strncpy(car.pn, carimg, sizeof(car.pn) - 1);
    strncpy(car.pnf, carimgf, sizeof(car.pnf) - 1);

    //add it to the queue
    Qenqueue(&car);
    sleep(3);
    while (true){
        show();
        sleep(1);
    }
    
    
    
    //[Test]: testing the GUI (monitor thread)
    //pthread_create(&monitor, NULL, run_monitor, NULL);    
    //pthread_join(monitor,NULL);
    
    return 0;
}














/**
    Read in args from argv
    @author Hesham T. Banafa
    @date Apr 17th, 2022
*/
inline void process_args(char *argv[], int argc, int *in_val, int *out_val, int *qsize, double *exp_cars)
{
    /* Redundant code, we can replace with macro or inline function */
    for (int i = 0; i < argc; i++) {
        if (i == 1) {
            int psize_in = atoi(argv[i]);
            if (psize_in > PARK_LIMIT) psize = PARK_LIMIT;
            else if (psize_in < 12)    psize = 12; 
            else                       psize = psize_in;

        } else if (i == 2) {
            int in_valets_in = atoi(argv[i]);
            if (in_valets_in > VALET_LIMIT) *in_val = VALET_LIMIT;
            else if (in_valets_in < 1)      *in_val = 1;
            else                            *in_val = in_valets_in;

        } else if (i == 3) {
            int out_valets_in = atoi(argv[i]);
            if (out_valets_in > VALET_LIMIT) *out_val = VALET_LIMIT;
            else if (out_valets_in < 1)      *out_val = 1; 
            else                             *out_val = out_valets_in;

        } else if (i == 4) {
            int qsize_in = atoi(argv[i]);
            if (qsize_in > QUEUE_LIMIT) *qsize = QUEUE_LIMIT;
            else if (qsize_in < 3)      *qsize = 3;
            else                        *qsize = qsize_in;

        } else if (i == 5) {
            double exp_car_in = atof(argv[i]);
            if (exp_car_in > 1.5)       *exp_cars = 0.05;
            else if (exp_car_in < 0.01) *exp_cars = 0.01;
            else                        *exp_cars = exp_car_in;
        }
    }
}

/**
    Print help in case of incorrect usage
    @author Hesham T. Banafa
    @date Apr 16th, 2022
*/
void usage()
{
    fprintf(stderr, "usage: carpark [psize inval outval qsize expnum]");
}

