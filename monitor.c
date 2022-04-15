#include "CPSimulator.h"

/** 
    Calculate car-park cumulative utilization
    @param  double ptr  varible of pervious calculation
    @param  double ptr to utilization variable
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A 
*/
inline void calc_utilization(double *ut_previous, double *ut) 
{
    double ut_now = oc / (double)psize;
    *ut = (*ut_previous + ut_now) / 2;
    *ut_previous = ut_now;
}

/**
    Print current status of the carpark
    @param void
    @return void
    @author Muhannad Al-Ghamdi
*/
void print_stats()
{
    // Print
}
