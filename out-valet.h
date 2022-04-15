
#include <pthread.h>

/**
    Initialize out-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Hesham T. Banafa 
*/
int init_in_valets(int number_valets);

/** 
    Entry point for out-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Hesham T. Banafa
*/
void *run_in_valet(void *args);
