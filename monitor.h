#include <pthread.h>




extern double ut;

/** 
    Entry point for monitor thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
    @data 15/04/2022
*/
void *run_monitor (void *args);

/** 
    Calculate car-park cumulative utilization
    @param  double ptr  varible of pervious calculation
    @param  double ptr to utilization variable
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A
    @data 15/04/2022
*/
void calc_utilization(double *ut_previous, double *ut);

/**
    Print current status of the carpark
    @param void
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A
    @data 15/04/2022
*/
void print_stats();
/** 
    terimantes the monitor thread
    @param  pthread_t fot the monitor thread 
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A 
    @data 25/04/2022
*/
void term_monitor(pthread_t tid);
