#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdatomic.h>
#include "in-valet.h"
#include "CarPark.h"
#include "CPSimulator.h"
#include "Queue.h"
#include "PriorityQueue.h"




/**
    Entry point for in-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
    @date 18/04/2022
*/

int num_in_valets = 0;
pthread_t *tid = NULL;
atomic_int turn_in;

void *run_in_valet(void *args){
    Car*  newCar;
    time_t delta;
    int id;
    id = (int64_t)args;
    turn_in =0;
    while (true){
        
        setViState(id, READY);	// Set the state of in-valet 
    
        /*get the car from the queue*/    
        sem_wait(&arrivals); /*wait for arrivals*/
        sem_wait(&mutex);
        
        /* A solution to prevent starvation and operate in a RR */
        if (turn_in != id) {
            /* Not our turn */
            sem_post(&arrivals); sem_post(&mutex); continue; /* Release lock and go back */
        }
        turn_in++;
        if (turn_in == num_in_valets) turn_in = 0; /* Reset to first thread */

        setViState(id, FETCH);	// fetching car from queue
        
        newCar = Qserve();
        setViCar(id, newCar);	// Set the car acquired by the in-valet   
        sem_post(&mutex);   /*release the queue lock*/
        newCar->vid=id;
        
        
        
        

        /*record the car wating time in the queue*/
        delta = time(NULL) - newCar->atm;
        sem_wait(&sqw_mutex); /*acquire the lock for Accumulation sum of car-waiting times in the queue*/
        sqw += delta;
        sem_post(&sqw_mutex); /*release the lock for Accumulation sum of car-waiting times in the queue*/


        /*update the number of cars currently acquired by in-valets*/
        sem_wait(&in_held_mutex); /*acquire the nm lock*/
        nm++;
        sem_post(&in_held_mutex); /*release the nm lock*/
        
        
        
        setViState(id, WAIT);	// Set the state of in-valet 
        /*park/serve the acquired car*/
        sem_wait(&empty); /*wait for empty slot in the parking area*/
        pthread_mutex_lock(&writer); /*acquire the parking array lock*/
        oc++;
        //find NULL locations in the parking array
        
        setViState(id, MOVE);
        usleep((int)(((double)rand() /RAND_MAX)*pow(10,6)));
        PQenqueue(newCar);
        newCar->sno=PQsize()-1;
        newCar->ptm=time(NULL);		// The time of parking (start time)

        pk++;

        //sem_post(&lock_parked); /*signal a new parked car*/
        pthread_mutex_unlock(&writer); /*release the parking array lock*/
        
        
        
        sem_wait(&in_held_mutex); /*acquire the queue lock*/
        nm--;
        sem_post(&in_held_mutex); /*release the nm lock*/
                
    }
}




/**
    Initialize in-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Muhannad Al-Ghamdi 
    @date 18/04/2022
*/
int init_in_valets(int number_valets) 
{
    num_in_valets = number_valets;
    tid = calloc(number_valets,sizeof(pthread_t));
    
    for (int i=0; i<number_valets; i++){
        pthread_create(&tid[i],NULL,run_in_valet,(void *)(int64_t)i);
    }
    
    return 0;
}

void term_invalets(){
     for (int i=0; i<num_in_valets; i++){
          pthread_cancel(tid[i]);
     }
}

