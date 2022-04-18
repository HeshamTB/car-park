#include "in-valet.h"
#include "CPSimulator.h"
#include "CPSimulator.h"
#include <pthread.h>
#include <semaphore.h>
#include <Queue.h>

int init_in_valets(int number_valets) 
{
    
    return 0;
}



void *run_in_valet(void *args){
    Car*  newCar;
    time_t delta;
 
    while (true){
        
    sem_wait(&arrivals); /*wait for arrivals*/   
    sem_wait(&mutex);    /*acquire the queue lock*/ 
    newCar = Qserve();   
    sem_post(&mutex);   /*release the queue lock*/
    
    
    
    delta = time(NULL) - newCar.atm;
    sem_wait(&sqw_mutex); /*acquire the lock for Accumulation sum of car-waiting times in the queue*/
    sqw += delta
    sem_post(&sqw_mutex); /*release the lock for Accumulation sum of car-waiting times in the queue*/


    sem_wait(&in_held_mutex); /*acquire the nm lock*/
    nm++
    sem_post(&in_held_mutex); /*release the nm lock*/
    
    
    
    
    
    sem_wait(&empty); /*wait for empty slot in the parking area*/
    sem_wait(&writer); /*acquire the parking array lock*/
    oc++
    //find NULL locations in the parking array
    // replace it with the new car
    sem_post(&parked); /*signal a new parked car*/
    sem_post(&writer); /*release the parking array lock*/
    
    
    
    

    sem_wait(&in_held_mutex); /*acquire the queue lock*/
    nm--;
    sem_post(&in_held_mutex); /*release the nm lock*/
    
    
}
