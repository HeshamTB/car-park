#ifndef PQUEUE_H
#define PQUEUE_H


#include <pthread.h>		// Provides thread-safe mutex lock
#include <stdbool.h>        // Provides boolean data type
#include "Car.h"			// Provides the required Car type structure

/* =============================================================================
 * FIFO Queue struct holds the queue array and its standard field variables.
 * To implement this queue write the standard queue operations with signatures
 * as listed below. [Qinit(), Qfree(), Qenqueue(), Qserve(), Qpeek(), Qsize(),
 * Qcapacity(), Qclear(), QisEmpty(), and QisFull()].
 *
 * Also use the extra field "list" to write function [Qiterator] that returns
 * a list used to traverse all the elements of the queue starting at the head
 * and ending at the tail without changing the state of the queue.
 * =============================================================================
 */
typedef struct PQueue_t {
    Car **data;       		// Array to hold car queue
    int capacity;      		// The array (queue) capacity
    int count;              // Number of cars currently in the queue
} PriorityQueue;

/* =============================================================================
 * Initialize the feilds of a Queue structure instance.
 * =============================================================================
 */
void PQinit(int n);

/* =============================================================================
 * Free the Queue data and list arrays.
 * =============================================================================
 */
void PQfree();

/* =============================================================================
 * Clear the Queue.
 * =============================================================================
 */
void PQclear();

/* =============================================================================
 * A FIFO Queue enqueue function.
 * Check precondition Qisfull() = false.
 * =============================================================================
 */
void PQenqueue(Car *car);

/* ===========================================================================
 * A FIFO Queue delete function.
 * Delete and return the car at Queue head.
 * Check precondition QisEmpty = false.
 * ===========================================================================
 */
Car* PQserve();

/* ===========================================================================
 * Return the car at the head of the Queue, without deleting it.
 * ===========================================================================
 */
Car* PQpeek();

/* ===========================================================================
 * Return the capacity of the FIFO Queue.
 * ===========================================================================
 */
int PQcapacity();

/* ===========================================================================
 * Return the number of cars in the FIFO Queue.
 * ===========================================================================
 */
int PQsize();

/* ===========================================================================
 * Return true if the FIFO Queue is full. Return false otherwise.
 * ===========================================================================
 */
bool PQisFull();

/* ===========================================================================
 * Return true if the FIFO Queue is empty. Return false otherwise.
 * ===========================================================================
 */
bool PQisEmpty();

#endif