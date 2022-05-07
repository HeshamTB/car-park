#ifndef INVALET
#define INVALET
#include <pthread.h>

/**
    Initialize in-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Muhannad Al-Ghamdi 
*/
int init_in_valets(int number_valets);

/** 
    Entry point for in-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
*/
void *run_in_valet(void *args);

/**
    termination of the in-valets threads
    @returns void
    @author Muhannad Al-Ghamdi
    @date 25/04/2022
*/
void term_invalets();

#endif