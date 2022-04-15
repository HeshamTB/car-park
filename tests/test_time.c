#include <stdio.h>
#include <time.h>

#include "Queue.h"

int main(void) 
{
    time_t sec;
    sec = time(NULL);
    printf("%s\n", ctime(&sec));
    sec = sec + 120;
    printf("%s\n", ctime(&sec));
    Qinit(5);
    Qenqueue(NULL);
}