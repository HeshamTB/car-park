#ifndef QUEUE_H
#define QUEUE_H


/*
 * Queue.h
 *
 * Header file for a FIFO queue structure using an array of
 * Car-type elements stored in it.
 *
 * The following function specifications shall be used to
 * implement the FIFO queue:
 *
 * - void  Qinit(int n)
 * - void  Qfree()
 * - void  Qclear()
 * - void  Qenqueue(Car *car)
 * - Car*  Qserve()
 * - Car*  Qpeek()
 * - Car** Qiterator(int *sz)
 * - int   Qcapacity()
 * - int   Qsize()
 * - bool  QisFull()
 * - bool  QisEmpty()
 *
 * Author Dr. Abdulghani M. Al-Qasimi
 * Version 2.00 2022/02/20
 *
 */

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
typedef struct Queue_t {
    Car **data;       		// Array to hold car queue
	Car **list;				// Array to hold car list
    int capacity;      		// The array (queue) capacity
    int count;              // Number of cars currently in the queue
    int tail;             	// The queue tail position
    int head;            	// The queue head position
} Queue;

/* =============================================================================
 * Initialize the feilds of a Queue structure instance.
 *  @param the queue capacity
 *  @returns void
    @author Hesham T. Banafa
    @date 15/04/2022
 * =============================================================================
 */
void Qinit(int n);

/* =============================================================================
 * Free the Queue data and list arrays.
 *  @returns void
    @author Hesham T. Banafa
    @date 15/04/2022
 * =============================================================================
 */
void Qfree();

/* =============================================================================
 * Clear the Queue.
    @returns void
    @author Hesham T. Banafa
    @date 15/04/2022
 * =============================================================================
 */
void Qclear();

/* =============================================================================
 * A FIFO Queue enqueue function.
 * Check precondition Qisfull() = false.
    @returns void
    @author Hesham T. Banafa
    @date 15/04/2022
 * =============================================================================
 */
void Qenqueue(Car *car);

/* ===========================================================================
 * A FIFO Queue delete function.
 * Delete and return the car at Queue head.
 * Check precondition QisEmpty = false.
    @returns a pointer to the served car
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
Car* Qserve();
/* ===========================================================================
 * Return the car at the head of the Queue, without deleting it.
    @returns a pointer to the car at the head
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
Car* Qpeek();

/* ===========================================================================
 * Return a list of the queue contents and its size.
    @returns a pointer to the list of cars
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
Car** Qiterator(int *sz);
/* ===========================================================================
 * Return the capacity of the FIFO Queue.
    @returns void
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
int Qcapacity();

/* ===========================================================================
 * Return the number of cars in the FIFO Queue.
 *  @returns the size of the queue
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
int Qsize();

/* ===========================================================================
 * Return true if the FIFO Queue is full. Return false otherwise.
 *  @returns true if the queue is full false if the queue is not full.
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
bool QisFull();

/* ===========================================================================
 * Return true if the FIFO Queue is empty. Return false otherwise.
    @returns true if the queue is empty false if not.
    @author Hesham T. Banafa
    @date 15/04/2022
 * ===========================================================================
 */
bool QisEmpty();

#endif