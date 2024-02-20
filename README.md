malloc() and free()
===================

## Names and RUIDs

Ron Cohen (rc1456) and Enrico Aquino (eja97)

## How to run the program


## Testing Plan
During our program we wanted to make sure that we don't use any in use memory / any address we return to the user should have not be in use prior to their request. To do this we need to have a way to determine if the chunk is in use or free. Also to test our mymallc() to make sure all other requests the user gives us are ok, this could be the size, being less than or equal to 0 or greater than the maximum size which is 4088. Another issue we need to test for is to make sure myfree() only trys to free memory that is in our heap, so we need to make sure the pointer given by the user is within the heap, then check to see if its valid by doing a linear search through our heap.

## Design choice
For our payload size we made it as a minimum size of 1 byte and a maximum size of 4088 bytes. We set a minimum size of 1 because if we have a 0 byte chunk, it will just take up extra space in the memory.

We also decided that in the header we will keep track of the payload size and if it was in use or not. This is because it keeps the header very small and we can still linearly search though the heap.

We also made the size keep try of the payload's size and not the entire size of the chunk. This lead to a lot of places us putting +8, however I don't think it would have made it more confusing for me if we did it the other way.

When the payload free space is 8 bytes less than what the user requested we give the user the extra 8 bytes left. This is because those 8 bytes would turn into a header for a free chunk that has a size of 0. I didn't want to make this a possibility because it's a check I use for the first chunk in the heap to see if it's initialized and I wasn't sure if I was going to need to use this check later on in the project.

Another design choice I made was to exit the program when mymalloc() has an error such as an invalid size or out of space. I also made myfree() return a NULL value if it encounters any issues when trying to free. Both functions print an error message with the file and line number of where the error occured to help the user.

## The Two Performance Tests (should have 5 total memgrind tests)
- Performance Test 1:
    We malloc 256 chunks and then free all 256 chunks in linear order. 
    This is just to test prev chunk coalesing.
- Performance Test 2:
    We malloc 256 chunks, then free all the even chunks, then free all the odd chunks.
    This is to make sure that our chunks have propper coalesing when the previous chunk and next chunk are already free.
- Performance Test 3: 
    We malloc 1 large chunk and free the large chunk.
    A basic test to see how long the best case scenario would take.
- Performance Test 4:
    Test how long it takes for our free() to tell us that we are trying to free an invalid pointer.
- Performance Test 5:
    WIP

## time differences between mymalloc and the real malloc

## Testing Cases
For testing, we are planning on to check how our mymalloc() and myfree() works under different circumstances:

- Filling up the chunks with a distinct size freeing every other object and then
    try to allocate a large chunk of memory (Expected result: Throw an error)


- Free an address that has already been freed (Expected result: Throw an error)
    We free a pointer given to us from malloc and then try to free the same pointer

- malloc 0 bytes (Expected result: Error, not enough memory requested)
    We make sure that the malloc size given is >0 and <=4088

- Free every even, then odd chunk (Expected result: No errors)
    This tests allows us to try coalescing the chunk to the left and chunk to the right of each odd chunk that gets free'd