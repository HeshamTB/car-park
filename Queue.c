#include <stdio.h>
#include <stdlib.h>
#include "CPSimulator.h"
#include "Queue.h"

/**
    This is a Queue implementaion using basic array, not linked lists
    Thus, head, tail are indicies to an array.
    Circular array-based queue

    TODO: correctly set value of tail
*/
static Queue queue;

/* =============================================================================
 * Initialize the feilds of a Queue structure instance.
 * =============================================================================
 */
void Qinit(int n)
{
    queue.capacity = n;
    queue.tail = 0;
    queue.head = 0;
    queue.count = 0;
    queue.list = malloc(sizeof(Car)*queue.capacity); // Returns a ptr to array of car ptrs
}

/* =============================================================================
 * Free the Queue data and list arrays.
 * =============================================================================
 */
void Qfree()
{

}

/* =============================================================================
 * Clear the Queue.
 * =============================================================================
 */
void Qclear()
{
    for (int i = 0; i < queue.capacity; i++) {
        queue.list[i] = NULL; 
    }
    Qinit(queue.capacity); // Should reset all 
}

/* =============================================================================
 * A FIFO Queue enqueue function.
 * Check precondition Qisfull() = false.
 * =============================================================================
 */
void Qenqueue(Car *car)
{
    if (QisFull()) {
        // Do not add. We cant throw execpt or retun a value...
        return;
    }
    int ix_new_head = (queue.head + queue.count) % queue.capacity;
    queue.count++;
    queue.list[ix_new_head] = car;
    queue.head = ix_new_head;
    //queue.count++;
}

/* ===========================================================================
 * A FIFO Queue delete function.
 * Delete and return the car at Queue head.
 * Check precondition QisEmpty = false.
 * ===========================================================================
 */
Car* Qserve()
{
    if (QisEmpty()) return NULL;
    Car *result = queue.list[queue.head]; // Temp save ptr. init outside.
    queue.list[queue.head] = NULL; // Clear slot
    queue.head = (queue.head + 1) % queue.capacity; // New head
    queue.count--;
    return result;
}

/* ===========================================================================
 * Return the car at the head of the Queue, without deleting it.
 * ===========================================================================
 */
Car* Qpeek()
{
    return queue.list[queue.head];
}

/* ===========================================================================
 * Return a list of the queue contents and its size.
 * ===========================================================================
 */
Car** Qiterator(int *sz)
{
    return NULL;
}

/* ===========================================================================
 * Return the capacity of the FIFO Queue.
 * ===========================================================================
 */
int Qcapacity()
{
    return queue.capacity;
}

/* ===========================================================================
 * Return the number of cars in the FIFO Queue.
 * ===========================================================================
 */
int Qsize()
{
    return queue.count;
}

/* ===========================================================================
 * Return true if the FIFO Queue is full. Return false otherwise.
 * ===========================================================================
 */
bool QisFull()
{
    return queue.count >= queue.capacity; // Should never be greater
}

/* ===========================================================================
 * Return true if the FIFO Queue is empty. Return false otherwise.
 * ===========================================================================
 */
bool QisEmpty()
{
    return queue.count == 0;
}

