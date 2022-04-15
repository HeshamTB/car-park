#include <pthread.h>

/** 
    Entry point for monitor thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
*/
void *run_monitor(void *args);

/** 
    Calculate car-park cumulative utilization
    @param  double ptr  varible of pervious calculation
    @param  double ptr to utilization variable
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A 
*/
void calc_utilization(double *ut_previous, double *ut);

/**
    Print current status of the carpark
    @param void
    @return void
    @author Muhannad Al-Ghamdi
*/
void print_stats();