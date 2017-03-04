#ifndef AAE_MALLOC_H
#define AAE_MALLOC_H
#include "misc/aae_types.h"
#define aae_malloc malloc 
#define aae_free free
#define aae_realloc realloc 
#define aae_calloc calloc 
/** allocate numbytes on the heap **/
extern "C" void* malloc(aae_size_t numbytes);
/** free memory segment on the heap starting at firstbyte **/
extern "C" void free(void * firstbyte);
/** allocate a new segment on the heap and copy old data **/
extern "C" void* realloc(void*firstbyte, aae_size_t numbytes);
/** allocate a segment on the heap with total_size = n * size, all bytes initialized to 0 **/
extern "C" void* calloc(size_t n, size_t size);
#endif
