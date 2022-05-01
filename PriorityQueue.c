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

/* =============================================================================
 * Initialize the feilds of a Queue structure instance.
 * =============================================================================
 */
void PQinit(int n)
{
    pqueue.capacity = n;
    pqueue.count = 0;
    pqueue.data = malloc(sizeof(Car)*pqueue.capacity); // Returns a ptr to array of car ptrs
}

/* =============================================================================
 * Free the Queue data and list arrays.
 * =============================================================================
 */
void PQfree()
{
    // The only runtime allocated memory are the list and data fields.
    free(pqueue.data);
}

/* =============================================================================
 * Clear the Queue.
 * =============================================================================
 */
void PQclear()
{
    for (int i = 0; i < pqueue.capacity; i++) {
        pqueue.data[i] = NULL; 
    }
    free(pqueue.data);
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
        pqueue.data[0]=car;
        pqueue.count++;
        return; 
    }
    
    for (int i=0 ; i<count;i++){
        if(car->ltm < pqueue.data[i]->ltm){
            for (int j=count ; j>i ; j--){
                pqueue.data[j]=pqueue.data[j-1];
            }
            pqueue.data[i]=car;
            pqueue.count++;
            return;
        }
    }
    pqueue.data[count]=car;
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
    Car *result = pqueue.data[0];
    pqueue.data[0]=NULL;
    pqueue.count--;
    for (int i =0; i<pqueue.count; i++){
        pqueue.data[i]=pqueue.data[i+1];
    }
    return result;
    
// 
//    
//    for (int i=0 ; i<counts;i++){
//        if (queue.data[i]==NULL) continue;
//        if(queue.data[i]->ltm < result->ltm){
//            result = queue.data[i];
//            idx = i;
//        }
//    }
//
//    queue.data[idx] = NULL; // Clear slot
//    queue.count--;
//    return result;
}

/* ===========================================================================
 * Return the car at the head of the Queue, without deleting it.
 * ===========================================================================
 */
Car* PQpeek()
{    
    return pqueue.data[0];
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

