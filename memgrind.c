#include <stdio.h>
//#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define AVGRUNCOUNT 500.0

int main(int argc, char **argsv) {
    int *piarr[256];
    
    struct timeval stop, start;

    gettimeofday(&start, NULL);

    //loop 50 times
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
    unsigned long totalTime = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("took %lu us\nAvg Time: %f us\n", totalTime, ((float)totalTime)/(AVGRUNCOUNT));

    return 0;
}

