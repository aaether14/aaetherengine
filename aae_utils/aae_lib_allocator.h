#ifndef AAE_LIB_ALLOCATOR_H
#define AAE_LIB_ALLOCATOR_H
#include "aae_types.h"
#ifdef AAE_USE_DEFAULT_ALLOCATOR
#define aae_malloc malloc 
#define aae_free free
#define aae_realloc realloc 
#define aae_calloc calloc 
#endif

/** allocate numbytes on the heap **/
extern "C" void* aae_malloc(aae_size_t numbytes);
/** free memory segment on the heap starting at firstbyte **/
extern "C" void aae_free(void * firstbyte);
/** allocate a new segment on the heap and copy old data **/
extern "C" void* aae_realloc(void*firstbyte, aae_size_t numbytes);
/** allocate a segment on the heap with total_size = n * size, all bytes initialized to 0 **/
extern "C" void* aae_calloc(size_t n, size_t size);
#endif
