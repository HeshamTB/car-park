#include <stdio.h>
#include <unistd.h>
#include "CPSimulator.h"
#include "CarPark.h"


double ut, ut_previous;

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
    double ut_now = 100*oc / (double)psize;
    *ut = (*ut_previous + ut_now) / 2;
    *ut_previous = ut_now;
}

/**
    Print current status of the carpark
    @param void
    @return void
    @author Muhannad Al-Ghamdi
    @precondition N/A
*/
void print_stats()
{
    printf("------------------------------[stats]-----------------------------------\n");
    printf("number of ocupied slots = %d\n",oc);
    printf("total number of created cars = %ld\n",nc);
    printf("total number of allowed to park cars = %ld\n",pk);
    printf("total number of not allowed to park cars = %ld\n",rf);
    printf("number of cars currently acquired by in-valets = %d\n",nm);
    printf("Accumulation sum of car-waiting times in the queue = %ld\n",sqw);
    printf("Accumulation sum of car-parking durations = %ld\n",spt);
    printf("The car-park space utilization = %0.2f\n",ut);
    printf("------------------------------------------------------------------------\n");

}

/** 
    Entry point for monitor thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
*/
void *run_monitor(void *args){
    ut = 0;
    ut_previous = 0;
    
    while(true){
        calc_utilization(&ut_previous, &ut);
        updateStats(oc, nc, pk, rf, nm, sqw, spt, ut);
        show();
        print_stats();
        sleep(1); // sleeps for 1s required in CarPark to complete drawing
    }
}