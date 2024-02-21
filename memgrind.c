#include <stdio.h>
//#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define AVGRUNCOUNT 50.0
    

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

/**
 * Default test 1 given in the p1.pdf
*/
void defaultTest1()
{
    struct timeval stop, start;
    unsigned long totalTime;
    gettimeofday(&start, NULL);
    int *obj;
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        for (int i = 0; i < 120; i++)
        {
            obj=malloc(1);
            free(obj);
        }
    }
    
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/(1000.0));
    printf("\nDefault Performance Test 1\nmalloc and free a 1 byte object 120 times\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}


/**
 * Default test 2 given in the p1.pdf
*/
void defaultTest2()
{
    int *piarr[120];
    struct timeval stop, start;
    unsigned long totalTime;

    gettimeofday(&start, NULL);
    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        for (int i = 0; i < 120; i++)
        {
            piarr[i] = (int *) malloc(1);
            *(piarr[i]) = i;
        }
        for (int i = 0; i < 120; i++)
        {
            free(piarr[i]);
        }
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/1000);
    printf("\nDefault Performance Test 2\nmalloc 1 byte object 120 times then free all the objects\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));

}





/**
 * Default test 3 given in the p1.pdf
*/
void defaultTest3()
{
    struct timeval stop, start;
    unsigned long totalTime;

    gettimeofday(&start, NULL);

    for (int j = 0; j < AVGRUNCOUNT; j++)
    {
        int *piarr[120];
        int allocCount = 0; // Count of allocations made
        int index=0;
        
        while (allocCount < 120) {
            // printf("%i\n",allocCount);
            if (rand() % 2 == 0) { // 50/50
                //allocate a 1-byte object and adding the pointer to the array 
                piarr[index]=malloc(1);
                allocCount++;
                index++;
            }
            else{
                //deallocating a previously allocated object (if any)
                if(index>0) {
                    index-=1;
                    free(piarr[index]);
                }
            }
        }
        
        for (int i = 0; i < index; i++) {
            free(piarr[i]);
        }
    }
    gettimeofday(&stop, NULL);
    totalTime = (stop.tv_sec - start.tv_sec)*1000 + ((stop.tv_usec - start.tv_usec)/(1000.0));
    printf("\nDefault Performance Test 3\nrandomly deallocate and allocate chunks\ntook %i ms\nAvg Time: %f ms\n", (int)totalTime, ((double)totalTime)/(AVGRUNCOUNT));
}





int main(int argc, char **argsv) {

    // uncomment the test you want to run
    printf("\n\n\nmemgrind testing\n\nuncomment the test you want to run.\n\nDefault tests 1-3 and Performance Tests 1-5\n\n\n");
    
    defaultTest1();
    defaultTest2();
    defaultTest3();
    perfTest1();
    perfTest2();
    perfTest3();

    // note tests 4 and 5 print a lot of info because free() on failure prints an error message.
    //perfTest4();
    //perfTest5();

    return 0;
}

