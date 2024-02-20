#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

#include "chunkhead.h"

#define MEMLENGTH 512
#define HEADER 8

#define mallocError(s) __mallocError(s, file, line)



#ifndef __DEBUG
#define __DEBUG 0
#endif
#define DEBUG if(__DEBUG)
#define LOG printf



static double memory[MEMLENGTH]; //4096 bytes, 512 doubles

#define HEAP ((char *)memory)

/**
 * View the heap
*/

// cursor-HEAP+4096
void viewHeap() 
{
    struct chunkhead *viewcursor = (chunkhead *) HEAP;
    while(viewcursor <= (chunkhead *)(HEAP+(MEMLENGTH*8)-8))
    {
        if((viewcursor->size>0 && viewcursor->inuse==0) || viewcursor->inuse == 1)
        {
            printf("\n\n\n");
            printf("---------------------------------\n");
            printf("| address      | %p \t|\n", viewcursor);
            printf("| TOTAL SIZE   | %i      \t|\n", viewcursor->size+8);
            printf("| INUSE        | %i      \t|\n", viewcursor->inuse);
            printf("| PAYLOAD SIZE | %i      \t|\n", viewcursor->size);
            printf("------------------------------------\n");
            printf("| ");

            for(int count=0; count < viewcursor->size/8; count++)
            {
                // when count is equal is divisable by 10
                if(count % 32 == 0 && count!=0)
                {
                    printf(" |\n| ");
                }
                printf("*");
            }

        
            printf(" |\n------------------------------------\n\n");
        }else
        if (viewcursor->inuse==0)
        {
            printf("\nfree: %p",(viewcursor));
        }
        viewcursor=(chunkhead *) (((char *)viewcursor)+(viewcursor->size+8));
    }
    
    printf("\nView complete\n\n\n");
    return;
}

void __mallocError(char* msg, char *file, int line)
{
    printf("\n\n\n\n\n");
    DEBUG viewHeap();
    printf("%s\n",msg);
    printf("Error (%s:%i)\n", file, line);
    return;
    //exit(1);
}

void *mymalloc(size_t size, char *file, int line) 
{
    //size can never be 0!! 0 size means initalize!
    if (((chunkhead *)HEAP)->size == 0 && ((chunkhead *)HEAP)->inuse == 0) 
    {
        // init heap
        struct chunkhead tempchunk={4096 - 8, 0};
        struct chunkhead *metadata = (chunkhead *)HEAP;
        *metadata = tempchunk; // put meta data at the start of the heap
        DEBUG LOG("init heap::inuse=%i::size=%i", metadata->inuse, metadata->size);
        DEBUG viewHeap();
    }

    // Exit Program if no memory is requested
    if(size <= 0)
    {
        mallocError("Not enough memory requested!");
        return NULL; 
    }

    DEBUG LOG("Requested size:%i\n", (int)size);
    size = (size+7) & ~7;
    DEBUG LOG("Changed to size:%i\n\n", (int)size);


    // Exit Program if size requested is bigger than maximum size
    if(size > (MEMLENGTH * 8) - 8)
    {
        mallocError("To much memory requested!");
        return NULL; 
    }    


    char *cursor = HEAP;
    char *bestFitPointer;
    int bestFitSize= 9999;
    int countError= 0;
    while(cursor < (HEAP + (MEMLENGTH * 8) - 8) && countError < MEMLENGTH * 8) 
    {
        countError += 1;
        // not in use
        DEBUG LOG("\naddress getting scanned:%p\n", cursor);
        DEBUG LOG("inuse=%i\t\t", ((chunkhead *)cursor)->inuse);
        DEBUG LOG("size=%i\n", ((chunkhead *)cursor)->size);
        // if the chunk is currently in use or
        // the current chunk size is smaller than required size
        if (((chunkhead *)cursor)->inuse == 1 || ((chunkhead *)cursor)->size < size)
        {
            DEBUG LOG("current chunk is in use or smaller\n");
        }

        // if current chunk is the exact size
        else if (((chunkhead *)cursor)->size == size)
        {
            DEBUG LOG("exact size\t\tcursor->size{%i}==size{%i}\n", ((chunkhead *)cursor)->size, (int)size);
            bestFitPointer = cursor;
            bestFitSize = size;
            break;
        }

        // if current chunk is smaller than bestFitSize
        else if ((((chunkhead *)cursor)->size) < bestFitSize)
        {
            DEBUG LOG("cursor size{%i} < bestFitSize{%i}\n", ((chunkhead *)cursor)->size, bestFitSize);
            bestFitPointer = cursor;
            bestFitSize = ((chunkhead *)cursor)->size;

            DEBUG LOG("cursor+8+((chunkhead *)cursor)->size{%p}\n", (cursor + (((chunkhead *)cursor)->size) + 8));
        }

        // move cursor forward
        cursor+=8+((chunkhead *)cursor)->size; 
    }
    DEBUG LOG("\n\nExited Search Loop\n\n");

    if (countError == MEMLENGTH * 8) {
        mallocError("countError 5000");
        return NULL;
    }

    // check if bestFit hasnt been found
    if(bestFitSize == 9999)
    {
        mallocError("Not enough memory. Memory fragmentation.");
        return NULL;
    }

    
    // quick way to calculate the remaining free bytes in the chunk that was best fit
    // payload of free chunk - requested size - 8 for the head
    int amtOfFreeLeftInChunk = bestFitSize - (size) - 8;

    // if the next chunk would have 0 bytes, give the 8 extra bytes to the chunk the user requested
    if (amtOfFreeLeftInChunk==0)
    {
        size+=8; // give the user the 8 extra bytes
    }

    // we have found best fit or exact fit
    // create a chunk head to put the meta data in
    struct chunkhead tempcurrentchunk = {size,1};
    // create a pointer of the chunk head type to the bestffit address in memory
    struct chunkhead *currentmetadata = (chunkhead *)bestFitPointer;
    // at that point in memory (the meta data area), input the tempcurrentchunk data.
    *currentmetadata = tempcurrentchunk; // put meta data at specific point in the heap

    DEBUG LOG("Created currentchunk meta data\ninuse{%i}::size{%i}::pointer{%p}\n",
                            currentmetadata->inuse,currentmetadata->size,(char *)currentmetadata);
    DEBUG LOG("CREATED CURRENT CHUNK META DATA\n");
 
 
 
    // for trailing chunk
    // ex: 
    // heap = 100000
    // bestFitPointer = 100400 
    // this means we are 400 bytes into the heap

    // distanceIntoHeap how big the last chunk was
    // ((size+8)+(size2+8)+(size3+8)...)
    // ex bestFitPointer(100040) - HEAP(100010) + size(payload size) + metadata size(8)
    int distanceIntoHeap = (bestFitPointer-HEAP)+size+8; 

    DEBUG LOG("distanceIntoHeap:%i\n",distanceIntoHeap);
    DEBUG LOG("amtOfFreeLeftInChunk:%i\n",amtOfFreeLeftInChunk);

    if(amtOfFreeLeftInChunk==0) {
        DEBUG LOG("amt of free in chunk == 0\n");
        DEBUG viewHeap();
        return (void *) (bestFitPointer+8);
    }

    // if the trailing chunk is still in the heap then create it
    if (distanceIntoHeap<4096) {
        struct chunkhead temptrailingchunk={amtOfFreeLeftInChunk,0};

        // we need it to start at the first byte of the new chunk
        struct chunkhead *trailingmetadata = (chunkhead *) (HEAP+distanceIntoHeap);

        // put meta data after current chunk / start of next chunk
        *trailingmetadata = temptrailingchunk; 

        DEBUG LOG("trailing metadata::inuse=%i::size=%i::address=%p\n",
                            trailingmetadata->inuse,trailingmetadata->size,(char *)trailingmetadata);
        DEBUG LOG("HEAP+distanceIntoHeap::inuse=%i::size=%i::address=%p\n",
                            ((chunkhead *)HEAP+distanceIntoHeap)->inuse,
                            ((chunkhead *)HEAP+distanceIntoHeap)->size,
                            (char *)(chunkhead *)(HEAP+distanceIntoHeap));
    } else {
        DEBUG LOG("Didn't create a trailing chunk\n");
    }
    DEBUG viewHeap();
    return (void *) (bestFitPointer+8);

}

/**
 * free()
 * params: ptr, file, line
 * return void
*/
void myfree(void *ptr, char *file, int line) {
    char *cursor = HEAP; 
    char *prevChunkCursor = HEAP;
    DEBUG LOG("Looking for chunk: %p\n",(char *)ptr-8);
    DEBUG LOG("HEAP START: %p\nHEAP END: %p\n",HEAP,HEAP+(MEMLENGTH*8));
    if ((char *)ptr<HEAP || (char *)ptr>HEAP+(MEMLENGTH*8))
    {
        mallocError("Free pointer not inside of heap!");
        return;
    }

    // search the heap 
    while(cursor < (HEAP+(MEMLENGTH*8)-8)) 
    {
        // if the cursor doesn't reach the correct chunk, increase counter 
        if(cursor+8 != ptr) 
        {
            prevChunkCursor = cursor;
            // next chunk
            cursor += 8+((chunkhead *)cursor)->size;
            DEBUG LOG("cursor: %p\nprevCursor: %p\n\n", cursor, prevChunkCursor);
            continue;
        }

        DEBUG LOG ("Chunk to free found: %p\n",cursor);
        DEBUG LOG ("prevChunkIs: %p\n",prevChunkCursor);

        if(((chunkhead *)cursor)->inuse==0)
        {
            mallocError("Free Error: object already free'd");
            return;
        }


        // set the nextChunkCursor as the cursor + payload size + 8
        char *nextChunkCursor = cursor + (((chunkhead *) cursor)->size+8);
        // coallese with next chunk if it is free and next chunk is inside of the heap
        if (((chunkhead*) nextChunkCursor)->inuse==0 && nextChunkCursor < HEAP+4096) {
            // increase the current chunk's payload by the next chunks payload + the meta data size
            ((chunkhead *)cursor)->size += ((chunkhead *) nextChunkCursor)->size + 8;
            DEBUG LOG("\nCoalescing w/ next chunk\nsize free'd: %i\n",((chunkhead *)cursor)->size);
        }

        // coallese with previous free chunk as long as current cursor is not the first chunk
        // this also makes sure that the current chunk is not at or the before the start of the HEAP
        if (((chunkhead *)prevChunkCursor)->inuse==0 && cursor!=HEAP)
        { 
            // increase the previous chunk's payload by the current chunk's payload + the meta data size
            ((chunkhead *)prevChunkCursor)->size += ((chunkhead *)cursor)->size+8;
            DEBUG LOG("\nCoalescing w/ previous chunk\nsize free'd: %i\n",((chunkhead *)prevChunkCursor)->size);

        } else {

            // if the prev chunk is not free make a new free chunk

            // create a chunkhead that has the same size payload as before, but set it to free
            struct chunkhead tempchunkhead={((chunkhead *)cursor)->size,0};
            // create a chunk pointer to the cursor's position
            struct chunkhead *ptempchunkhead = (chunkhead *)cursor;

            // overwrite the chunkhead that had data, with the free chunk + free chunk meta information
            *ptempchunkhead = tempchunkhead; 
            DEBUG LOG("prev chunk not free, changing current chunk to free and size is: %i\n",ptempchunkhead->size);

        }

        // reached end of heap
        DEBUG viewHeap();
        mallocError("Free Error: object not found");
        return;
    }

    // Unable to locate complete object, the pointer given and none of the pointers to the chunk's payload  are the sam
    DEBUG LOG ("cursor:%p\ninuse:%i\tsize:%i\nprevCursur:%p\ninuse:%i\tsize:%i\n", 
        cursor, ((chunkhead *)cursor)->inuse,((chunkhead *)cursor)->size,prevChunkCursor,((chunkhead *)prevChunkCursor)->inuse,((chunkhead *)prevChunkCursor)->size);
    mallocError("Free Error: unable to locate complete object");
    return;

}
