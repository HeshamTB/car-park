#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){

    usleep((int)(((double)rand() /RAND_MAX)*1000));
    printf("hey");
    return 0;
}