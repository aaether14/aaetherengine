#ifndef AAE_LIB_ALLOCATOR_H
#define AAE_LIB_ALLOCATOR_H



#include "aae_types.h"
#include "aae_defines.h"



#ifdef __cplusplus
AAE_EXTERN_TOOL {
#endif



/**
*allocate numbytes on the heap
*/
void * aae_malloc(aae_size_t numbytes);
/**
*free memory segment on the heap starting at firstbyte
*/
void aae_free(void * firstbyte);
/**
*Check how much memory has OS allocated on the heap
*/
aae_size_t aae_allocated_memory();



#ifdef __cplusplus
}
#endif




#endif
