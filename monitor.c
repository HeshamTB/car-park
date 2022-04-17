#include "CPSimulator.h"
#include "CarPark.h"
#include "CPsimulator.h"



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
    @precondition N/A
*/
void print_stats()
{
    printf("------------------------------[stats]-----------------------------------");
    printf("number of ocupied slots = %d\n",oc);
    printf("total number of created cars = %d\n",nc);
    printf("total number of allowed to park cars = %d\n",pk);
    printf("total number of not allowed to park cars = %d\n",rf);
    printf("number of cars currently acquired by in-valets = %d\n",nm);
    printf("Accumulation sum of car-waiting times in the queue = %d\n",sqw);
    printf("Accumulation sum of car-parking durations = %d\n",spt);
    printf("The car-park space utilization = %d\n",ut);
    printf("------------------------------------------------------------------------");

}





/** 
    Entry point for monitor thread
    @param Pointer to thread arguments array
    @returns void
    @author Muhannad Al-Ghamdi
*/
void *run_monitor(void *args){
    double *ut = 0;
    double *ut_previous = 0;
    
    while(true){
        calc_utilization(ut_previous,ut);
        show();
        sleep(1); // sleeps for 1s required in CarPark to complete drawing
        print_stats();
    }
}