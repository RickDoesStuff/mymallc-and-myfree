#include <stdio.h>
#include <stdlib.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

void rickysTest(){
    
    int *parrint = malloc(8*sizeof(int));

    parrint[0]=1;
    parrint[1]=54;
    parrint[2]=23;
    parrint[3]=99;
    parrint[4]=8;

    printf("\n------------------------\nnew\n------------------------\n\n");
    double *pdarrmalloc = malloc(4*sizeof(double));

    printf("\n------------------------\nnew\n------------------------\n\n");

    char *name = malloc(60*sizeof(char));

    printf("\n\nMalloc done\n\n");


    for(int i=0; i<5;i++)
    {
        printf("parrint[%i]\n",parrint[i]);
    }

    free(pdarrmalloc);

    double *pnewddouble=malloc(sizeof(double));

    printf("pnewint malloc\n\n");
    int *pnewint=malloc(sizeof(int));


    free(parrint);
    free(pnewddouble);
}

void rickysTest2(){
	    //bookTest();
    int *piarr[512];
    for (int i = 0; i<8; i++)
    {
        printf("piarr[%i]\n",i);
        piarr[i] = (int *) malloc(248+256);
    }
    
    // free invalid memory
    // printf("\n\nfree[15]+16\n");
    // free(piarr[15]+16);
    // test passes


    // free'ing already free'd test
    // int *piarr=piarr[1];
    // printf("\n\nfree[1]\n");
    // free(piarr[1]);
    // printf("\n\nfree[1] again\n");
    // free(piarr15[1]);
    // test passes

    // free memory not in heap
    // int test = 5;
    // int *ptest = &test;
    // free(ptest);
    // test passes

    // free memory not in heap
    // free(piarr[15]+128);
    // passes

    // test free first then the following chunk
    // free(piarr[0]);
    // free(piarr[1]);
    // passes

    // free 0, 2, then 1
    // free(piarr[0]);
    // free(piarr[2]);
    // free(piarr[1]);
    // passes

    // free 2, 4, then 3 in the middle of the heap
    // free(piarr[2]);
    // free(piarr[4]);
    // free(piarr[3]);
    // passes


    // free 2, 4, then 3 in the middle of the heap
    // free(piarr[5]);
    // free(piarr[7]);
    // free(piarr[6]);
    // passes? most times


    // free 5, then 6 then 7
    free(piarr[5]);
    free(piarr[6]);
    free(piarr[7]);
    // passes? most times
}

void check_objects(char *obj[OBJECTS]) {
	int i, j, errors = 0;
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < OBJSIZE; j++) {
			if (obj[i][j] != i) {
				errors++;
                printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}

	}
	printf("%d incorrect bytes\n", errors);
    if (errors == 0) {
        printf("Success!\n");
    }
}

void fill_objects(char *obj[OBJECTS]) {
	// fill memory with objects
	for (int i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}
	
	// fill each object with distinct bytes
	for (int i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, OBJSIZE);
	}

	check_objects(obj);
}

void free_every_other_object(char *obj[OBJECTS]) {
	// free every other object
    int totalFreed = 0;
	for (int i = 0; i < OBJECTS; i += 2) {
		free(obj[i]);
		printf("Object %d freed\n", i);
        totalFreed++;
	}
    if (totalFreed == OBJECTS/2) {
        printf("Every other object freed!\n");
    }
	check_objects(obj);
}

void malloc_large_object(char *obj[OBJECTS]) {
	for (int i = 0; i < OBJSIZE; i++) {
		obj[i] = malloc(MEMSIZE/2);
		if (obj[i] == NULL) {
			// printf("Cannot allocate large object\n");
            continue;
		} else {
			printf("ERROR: Large object allocated\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("Large object not allocated!\n");
}

void free_all(char *obj[OBJECTS]) {
    // free all objects
    for (int i = 0; i < OBJECTS; i++) {
        free(obj[i]);
    }
    check_objects(obj);
}

void free_odd_even(char *obj[OBJECTS]) {
    // free every odd then even objects
    for (int i = 0; i < OBJECTS; i += 2) {
        free(obj[i]);
    }
    check_objects(obj);
    for (int i = 1; i < OBJECTS; i += 2) {
        free(obj[i]);
    }
    check_objects(obj);
}

int main(int argc, char **argv)
{
	char *obj[OBJECTS];
	
	// fill memory with objects
	fill_objects(obj);

	// free every other object
	// free_every_other_object(obj);
    // test passes

	// malloc large object when memory is fragmented
	// malloc_large_object(obj);
    // test passes
	
    // free an address that has already been freed    
    // free the rest of the objects
    // free_all(obj);
    // test passes

    // malloc 0 bytes
    // malloc(OBJSIZE*0);

    // free every odd then even objects
    free_odd_even(obj);
    // test passes
	
	// rickysTest();
	//rickysTest2();

	return EXIT_SUCCESS;
}
