#include "Car.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int count = 0;

#define IMG_FILE_BASE ".cars/Car%d.bmp"
#define IMG_FILE_BASE_F ".cars/Car%d.bmp"

#define IMG_FILE_NAME_LIMIT 20

/** 
    Random init for new car
*/
void CarInit(Car *car)
{
    car->cid = count++;
    car->sno = 0;
    car->atm = time(NULL);

    /* random between 1 and 180 */
    car->ltm = rand() % 180;

    get_rand_images(car->pn, car->pnf);    

}

void get_rand_images(char *pn, char *pnf)
{
    int car_num = rand() % 12 + 1;
    sprintf(pn, IMG_FILE_BASE, car_num);
    sprintf(pnf, IMG_FILE_BASE_F, car_num); // Risky if car_num make totoal string exeed 20 chars
}