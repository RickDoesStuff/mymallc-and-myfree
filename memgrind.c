#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//#include "mymalloc.h"
#include <windows.h>


#define AVGRUNCOUNT 500.0
    

/**
 * Performance Test 1
 * loop 50 times create 256 chunks and free 256 chunks
 */
void perfTest1(){
    int *piarr[256];
    struct timeval stop, start;
    unsigned long totalTime;

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
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/1000);
    printf("\nPerformance Test 1\ncreate 256 chunks and free 256 chunks\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));

}

/**
 * Performance Test 2
 * loop 50 times create 256 chunks and free 256 chunks (first free the even then the odd)
 */
void perfTest2()
{
    int *piarr[256];
    struct timeval stop, start;
    unsigned long totalTime;

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
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/1000);
    printf("\nPerformance Test 2\ncreate 256 chunks and free 256 chunks (first free the even then the odd)\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}

/**
 * Performance Test 3
 * malloc 1 large chunk and free it
 */
void perfTest3()
{
    struct timeval stop, start;
    unsigned long totalTime;
    char *charPointer;
    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        charPointer = malloc(4088);
        free(charPointer);
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/(1000));
    printf("\nPerformance Test 3\ncreate 1 large chunk and free it\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}


/**
 * Performance Test 4
 * Test how long myfree() takes with double freeing 
 */
void perfTest4()
{
    struct timeval stop, start;
    unsigned long totalTime;
    char *charPointer = malloc(1);
    free(charPointer);
    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        free(charPointer);
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/1000);
    printf("\nPerformance Test 4\ncreate one chunk and double free it\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}

/**
 * Performance Test 5
 * Test how long myfree() takes when provided an address not on the heap
 */
void perfTest5()
{
    struct timeval stop, start;
    unsigned long totalTime;
    int x=5;
    int *xp=&x;
    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        free(xp);
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/(1000.0));
    printf("\nPerformance Test 5\nfree an address not on the heap\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}

int main(int argc, char **argsv) {

    // uncomment the test you want to run
    perfTest1();
    perfTest2();
    perfTest3();
    //perfTest4();
    //perfTest5();


    



    return 0;
}

