#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PriorityQueue.h"
#include "Car.h"
#include "CarPark.h"

int main(){
    PQinit(5);
    
    Car *car = calloc(1, sizeof(Car));;
    car->atm = 44;
    car->ltm = 54;
    strcpy(car->pnf, "test string");
    PQenqueue(car);    
    
    printf("Current number of cars in PQUEUE : %d\n",PQsize());
    
    car = calloc(1, sizeof(Car));;
    
    
    car->atm = 44;
    car->ltm = 1;
    strcpy(car->pnf, "test string");
    PQenqueue(car);    
    
    printf("Current number of cars in PQUEUE : %d\n",PQsize());
    
    Car *served = PQserve();
    
    printf("the served car has ltm : %d\n",served->ltm);
    
    served = PQserve();
    
    printf("the served car has ltm : %d\n",served->ltm);
    
    return -1;
}