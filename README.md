malloc() and free()
===================

## Names and RUIDs

Ron Cohen (rc1456) and Enrico Aquino (eja97)

## Testing Plan

For testing, we are planning on to check how our malloc() and free() works under different circumstances:

- Filling up the chunks with a distinct size freeing every other object and then
    try to allocate a large chunk of memory
    (Expected result: Throw an error)

- Free an address that has already been freed
    (Expected result: Throw an error)

- malloc 0 bytes (Expected result: Error, not enough memory requested)

- Free every even, then odd chunk (Expected result: No errors)
