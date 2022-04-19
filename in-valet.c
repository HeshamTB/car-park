#include <pthread.h>
#include <pthread.h>
#include <semaphore.h>
#include "in-valet.h"
#include "CarPark.h"
#include "CPSimulator.h"
#include "Queue.h"








/**
    Entry point for in-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
    @date 18/04/2022
*/
void *run_in_valet(void *args){
    Car*  newCar;
    time_t delta;
    int id;
    id = (int64_t)args;
    while (true){
        
        
    /*get the car from the queue*/    
    sem_wait(&arrivals); /*wait for arrivals*/   
    pthread_mutex_lock(&mutex);
    setViState(id, FETCH);	// Set the state of in-valet 
    newCar = Qserve();
    setViCar(id, newCar);	// Set the car acquired by the in-valet   
    pthread_mutex_unlock(&mutex);
   /*release the queue lock*/
    

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
    sem_wait(&writer); /*acquire the parking array lock*/
    oc++;
    //find NULL locations in the parking array
    for (int i=0; i<psize; i++){
        if (car_parks[i] == NULL){
            setViState(id, MOVE);	// Set the state of in-valet 
            // replace it with the new car
            car_parks[i] = newCar;
            break;
        }
    }
    sem_post(&lock_parked); /*signal a new parked car*/
    sem_post(&writer); /*release the parking array lock*/
    
    
    
    

    sem_wait(&in_held_mutex); /*acquire the queue lock*/
    nm--;
    sem_post(&in_held_mutex); /*release the nm lock*/
    setViState(id, READY);	// Set the state of in-valet 
    
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
    pthread_t tid[number_valets];
    
    for (int i=0; i<number_valets; i++){
        pthread_create(&tid[i],NULL,run_in_valet,(void *)(int64_t)i);
    }
    
    for (int i=0; i<number_valets; i++){
         pthread_join(tid[i],NULL);
    }
    
    
    return 0;
}

