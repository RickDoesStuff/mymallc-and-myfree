#include <stdio.h>
//#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define AVGRUNCOUNT 500.0

int main(int argc, char **argsv) {
    int *piarr[256];
    
    struct timeval stop, start;
    unsigned long totalTime;

    //loop 50 times create 256 chunks and free 256 chunks
    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            piarr[i] = (int *) malloc(8);
            *(piarr[i]) = i;
        }
        for (int i = 0; i < 256; i++)
        {
            free(piarr[i]);
        }
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("\nCreate 256 chunks and free 256 chunks\ntook %lu us(micro seconds)\nAvg Time: %f us(micro seconds)\n", totalTime, ((float)totalTime)/(AVGRUNCOUNT));


    //loop 50 times create 256 chunks and free 256 chunks (first free the even then the odd)
    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        for (int i = 0; i < 256; i++)
        {
            piarr[i] = (int *) malloc(8);
            *(piarr[i]) = i;
        }
        for (int i = 0; i < 256; i+=2)
        {
            free(piarr[i]);
        }
        for (int i = 1; i < 256; i+=2)
        {
            free(piarr[i]);
        }
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("\ncreate 256 chunks and free 256 chunks (first free the even then the odd)\ntook %lu us(micro seconds)\nAvg Time: %f us(micro seconds)\n", totalTime, ((float)totalTime)/(AVGRUNCOUNT));


    return 0;
}

