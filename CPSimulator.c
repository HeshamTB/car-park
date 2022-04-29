#include <alloca.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>
#include <signal.h>


#include "CarPark.h"
#include "CPSimulator.h"
#include "in-valet.h"
#include "out-valet.h"
#include "monitor.h"
#include <string.h>

int oc, nm, psize, in_valets, out_valets, qsize;
long nc, pk, rf, sqw, spt;
double exp_cars;
Car** car_parks = NULL;
struct tm st_tm ;

pthread_mutex_t writer;
sem_t mutex, arrivals, sqw_mutex, in_held_mutex, empty, lock_parked, spt_mutex;

pthread_t monitor;
pthread_attr_t monitor_thread_attr;

time_t start_time;


atomic_int inturppted = 0;

/**
    intializes the semaphores, the car park array, the car queue and the GUI
    @author Muhannad Al-Ghamdi
    @date Apr 18th, 2022
*/
void init(){
    //init the car Queue
    Qinit(qsize);
    srand(time(NULL));
    
    //init semaphores
    sem_init(&arrivals, 0, 0); // Arrival should be 'posted' after a Qenqueue
    sem_init(&sqw_mutex, 0, 1);
    sem_init(&in_held_mutex, 0, 1);
    sem_init(&empty, 0, psize);
    sem_init(&mutex, 0, 1);
    sem_init(&lock_parked, 0, 0);
    sem_init(&spt_mutex, 0, 1);
    
    //init the mutex for car park
    pthread_mutex_init(&writer,NULL);    
    //init car park array
    car_parks = calloc(psize,sizeof(Car));
    
    // setup interrupt handlers
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    //init the GUI
    G2DInit(car_parks, psize, in_valets, out_valets, writer);

    // start monitor thread
    pthread_attr_init(&monitor_thread_attr);
    pthread_create(&monitor, &monitor_thread_attr, run_monitor, NULL);

    if (init_in_valets(in_valets)) {
        perror("Failed to start in-valet threads\n");
        exit(EXIT_FAILURE);
    }

    if (init_out_valets(out_valets)){
        perror("Failed to start out-valet threads\n");
        exit(EXIT_FAILURE);
    }
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
    start_time = time(NULL);
    st_tm = *localtime(&start_time);
    
    
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
    exp_cars = EXP_CARS;

    /* Process arguments */
    process_args(argv, argc, &in_valets, &out_valets, &qsize, &exp_cars);
    printf("%d %d %d %d %.2f\n", psize, in_valets, out_valets, qsize, exp_cars);
    /* if some of the optional args are not set to a non-zero, init with default */
 



    init();
    
    while (1) {
        sleep(1);
        int num_newcars = newCars(exp_cars);
        /* allocate each car individually so we can later free per car */
        for (int i = 0; i < num_newcars; i++) {
            Car *new_car = calloc(1, sizeof(Car));
            
            CarInit(new_car);
            new_car->cid=nc+1;
            
            
            nc++;
            /* At this point a new car has arrived  (time is recored for waiting..) */
            if (QisFull()) {
                rf++; // Does not require sync
                free(new_car);
                continue; // Car is turned away
            }
            /* Aquire queue write lock */
            sem_wait(&mutex);
            Qenqueue(new_car);
            sem_post(&mutex);
            sem_post(&arrivals); // Signal an arrival
            /* A car is added to the queue */
        }
    }


    return 0;
}


/**
    Read in args from argv
    @author Hesham T. Banafa
    @date Apr 17th, 2022
*/
void process_args(char *argv[], int argc, int *in_val, int *out_val, int *qsize, double *exp_cars)
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



void clean_up(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    printf("\n\n------------------------------[SUMMARY]-----------------------------------\n");
    printf("%d-%02d-%02d %02d:%02d:%02d     :   recieved shutdown signal. \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    t = time(NULL); tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d     :   car park is shutting down \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    t = time(NULL); tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d     :   The valets are leaving. \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    term_invalets();
    term_outvalets();
    t = time(NULL);tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d     :   done %d valets left. \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,(in_valets+out_valets));
    term_monitor(monitor);
    t = time(NULL);tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d     :   monitor exiting... \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,(in_valets+out_valets));
    
    printf("Simulation started at %d-%02d-%02d %02d:%02d:%02d \n", st_tm.tm_year + 1900, st_tm.tm_mon + 1, st_tm.tm_mday, st_tm.tm_hour, st_tm.tm_min, st_tm.tm_sec);

    printf("Park Space Capacity was:    %d cars.\n",psize);
    printf("Allowed queue length was:    %d cars.\n",qsize);
    printf("Number of in valets was:    %d.\n",in_valets);
    printf("Number of out valets was:    %d.\n",out_valets);
    printf("Expected arrivals was:    %.2f.\n",exp_cars);
    
    t = time(NULL);tm = *localtime(&t);
    printf("Simulation stopped at:      %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,(in_valets+out_valets));
    
    printf("CP Simulation was executed for:     %d seconds\n",t-start_time);
    printf("Total number of cars processed:     %d\n",nc);
    printf("Number of cars that parked:     %d\n",pk);
    printf("Number of cars turned away:     %d\n",rf);
    printf("Number of cars in transit:     %d\n",nc-(pk+rf+Qsize()));  //check if it right (the cars that are with the valets)
    printf("Number of cars still queued:     %d\n",Qsize());
    printf("Number of cars still parked:     %d\n",oc);

    printf("Average queue waiting time:     %.3f\n",sqw);
    printf("Average parking time:     %.3f\n",spt);
    printf("Percentage of park utilization:     %.3f%\n",ut);
    
    /*TODO:
     * [] check for memory leaks
     * */

    finish();



    
}



// TODO: Implement handlers to clean up and print final report to terminal (stdout)
void sigint_handler()
{
    printf("Recieved SIGINT...\n");
    inturppted = 1;
    clean_up();
    exit(0);
}

void sigterm_handler()
{
    printf("Recieved SIGTERM...\n");
    inturppted = 1;
    clean_up();
    exit(0);
}



