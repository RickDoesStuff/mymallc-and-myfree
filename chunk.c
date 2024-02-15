#include <stdio.h>
#include <stdlib.h>

#include "chunkhead.h"

/**
 * header:struct chunkhead
 * payload:double*
*/
typedef struct chunk {
	struct chunkhead header; // 8 bytes
	double* payload; // multiple of 8 bytes
} chunk;