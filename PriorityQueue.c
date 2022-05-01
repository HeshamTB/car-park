#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "PriorityQueue.h"

/**
    This is a Queue implementaion using basic array, not linked lists
    Thus, head, tail are indicies to an array.
    Circular array-based queue
*/
static PriorityQueue pqueue;

struct CarNode {
   int idx;
   Car *car;
};

struct CarNode *data; 

/* =============================================================================
 * Initialize the feilds of a Queue structure instance.
 * =============================================================================
 */
void PQinit(int n)
{   
    pqueue.capacity = n;
    pqueue.count = 0;
    pqueue.list = malloc(sizeof(Car)*pqueue.capacity); // Returns a ptr to array of car ptrs
    data = calloc(pqueue.capacity,sizeof(struct CarNode));
    
    for (int i = 0; i < pqueue.capacity; i++) {
        pqueue.list[i] = NULL; 
    }

    
}

/* =============================================================================
 * Free the Queue data and list arrays.
 * =============================================================================
 */
void PQfree()
{
    // The only runtime allocated memory are the list and data fields.
    free(data);
    free(pqueue.list);
}

/* =============================================================================
 * Clear the Queue.
 * =============================================================================
 */
void PQclear()
{

    for (int i = 0; i < pqueue.capacity; i++) {
        pqueue.list[i] = NULL; 
    }
    free(data);
    free(pqueue.list);
    PQinit(pqueue.capacity); // Should reset all 
}

/* =============================================================================
 * A FIFO Queue enqueue function.
 * Check precondition Qisfull() = false.
 * =============================================================================
 */
void PQenqueue(Car *car)
{
    
    if (PQisFull()) {
        return;
    }
    
    int count = pqueue.count;
    
    if (count==0){
        struct CarNode new_car;
        new_car.car=car;
        new_car.idx=0;
        
        data[0]=new_car;
        pqueue.list[new_car.idx]=car;
        pqueue.count++;
        return; 
    }
    
    for (int i=0 ; i<count;i++){
        if(car->ltm < data[i].car->ltm){
            for (int j=count ; j>i ; j--){
                data[j]=data[j-1];
            }
            
            struct CarNode new_car;
            new_car.car=car;
            new_car.idx=count;
        
            data[i]=new_car;
            pqueue.list[new_car.idx]=car;
            
            pqueue.count++;
            
            return;
        }
    }
    
    struct CarNode new_car;
    new_car.car=car;
    new_car.idx=count;

    data[count]=new_car;
    pqueue.list[new_car.idx]=car;
    
    pqueue.count++;
    
}

/* ===========================================================================
 * A FIFO Queue delete function.
 * Delete and return the car at Queue head.
 * Check precondition QisEmpty = false.
 * ===========================================================================
 */
Car* PQserve()
{
    if (PQisEmpty()) return NULL;
    
    
    Car *result = data[0].car;
    int idx = data[0].idx;
    
    pqueue.list[idx]=NULL;
    
    struct CarNode new_car;
    data[0]= new_car;
    pqueue.count--;
    
    for (int i =0; i<pqueue.count; i++){
        data[i]=data[i+1];
    }
    return result;
    

}

/* ===========================================================================
 * Return the car at the head of the Queue, without deleting it.
 * ===========================================================================
 */
Car* PQpeek()
{    
    return data[0].car;
}


/* ===========================================================================
 * Return the capacity of the FIFO Queue.
 * ===========================================================================
 */
int PQcapacity()
{
    return pqueue.capacity;
}

/* ===========================================================================
 * Return the number of cars in the FIFO Queue.
 * ===========================================================================
 */
int PQsize()
{
    return pqueue.count;
}

/* ===========================================================================
 * Return true if the FIFO Queue is full. Return false otherwise.
 * ===========================================================================
 */
bool PQisFull()
{
    return pqueue.count >= pqueue.capacity; // Should never be greater
}

/* ===========================================================================
 * Return true if the FIFO Queue is empty. Return false otherwise.
 * ===========================================================================
 */
bool PQisEmpty()
{
    return pqueue.count == 0;
}

/* ===========================================================================
 * Return a list of the queue contents and its size.
 * ===========================================================================
 */
Car** PQgetList()
{
    return pqueue.list;
}


