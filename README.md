malloc() and free()
===================

# Names and RUIDs

Ron Cohen (rc1456) and Enrico Aquino (eja97)

# Testing Plan

For testing, we are planning on to check how our malloc() and free() works under different circumstances:

- Filling up the chunks with a distinct size freeing every other object and then
    try to allocate a large chunk of memory
    (Expected result: Throwing an error)

- Free Fragmented Memory/free an address that as already been freed 
    (Expected results: Should throw errors but should also free up the rest of the chunks)

- Free every even, then odd chunk
    (Expected results: No errors)