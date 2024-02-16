# Define the compiler
CC=gcc

# Compiler flags
CFLAGS=-Wall -g 

# Debugging flags
DFLAGS=-Wall -g -D__DEBUG

# Define the target executable name
TARGET_MEMTEST=memtest
TARGET_MEMGRIND=memgrind

# List of source files for memgrind
SOURCES_MEMGRIND=chunkhead.c chunk.c mymalloc.c memgrind.c

# Object files to generate for memgrind
OBJECTS_MEMGRIND=$(SOURCES_MEMGRIND:.c=.o)

# Default rule to build the memgrind
memgrind: $(TARGET_MEMGRIND)
	.\$(TARGET_MEMGRIND)
	make clean

# Debug rule to build the program memgrind with debug flags
debug_memgrind: CFLAGS = $(DFLAGS)
debug_memgrind: $(TARGET_MEMGRIND)
	.\$(TARGET_MEMGRIND)
	make clean

# Rule for linking the program
$(TARGET_MEMGRIND): $(OBJECTS_MEMGRIND)
	$(CC) $(CFLAGS) -o $@ $^

# List of source files for memtest
SOURCES_MEMTEST=chunkhead.c chunk.c mymalloc.c memtest.c

# Object files to generate for memgrind
OBJECTS_MEMTEST=$(SOURCES_MEMTEST:.c=.o)

# Default rule to build the memtest
memtest: $(TARGET_MEMTEST)
	.\$(TARGET_MEMTEST)
	make clean
	
# Test rule for building the program memtest with debug flags
debug_memtest: CFLAGS = $(DFLAGS)
debug_memtest: $(TARGET_MEMTEST)
	.\$(TARGET_MEMTEST)
	make clean

# Rule for linking the program
$(TARGET_MEMTEST): $(OBJECTS_MEMTEST)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for compiling source files to object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

# Special case for memtest.c which might not need a memtest.h
memtest.o: memtest.c
	$(CC) $(CFLAGS) -c $<
# Special case for memgrind.c which might not need a memgrind.h
memgrind.o: memgrind.c
	$(CC) $(CFLAGS) -c $<

# Rule for cleaning up
clean:
	del -f $(OBJECTS_MEMGRIND) $(OBJECTS_MEMTEST)

# Dependencies
chunkhead.o: chunkhead.c chunkhead.h
chunk.o: chunk.c chunk.h
mymalloc.o: mymalloc.c mymalloc.h
memtest.o: memtest.c memtest.h
memgrind.o: memgrind.c memgrind.h # Add this line if memgrind.h exists and is used