
#include <pthread.h>

/**
    Initialize out-valet thread-pool
    @param Number of valets to start
    @returns 0 if successful, 1 if failed
    @author Hesham T. Banafa
    @date 18/04/2022 
*/
int init_out_valets(int number_valets);

/** 
    Entry point for out-valet thread
    @param Pointer to thread arguments array
    @returns void
    @author Hesham T. Banafa
    @date 18/04/2022
*/
void *run_out_valets(void *args);

/** 
    terminates the out-valet theads
    @returns void
    @author Muhannad Alghamdi
    @date 25/04/2022
*/
void term_outvalets();