#ifndef CHUNK_H
#define CHUNK_H

#include <stdio.h>
#include <stdlib.h>
#include "chunkhead.h"

/**
 * header:struct chunkhead
 * payload:double*
*/
typedef struct chunk {
	struct chunkhead head; // 8 bytes
	double *payload; // multiple of 8 bytes
}chunk;
#endif
