#ifndef AAE_LIB_ALLOCATOR_H
#define AAE_LIB_ALLOCATOR_H


#include "aae_types.h"
#include "aae_defines.h"


/** allocate numbytes on the heap **/
AAE_EXTERN_TOOL void* aae_malloc(aae_size_t numbytes);
/** free memory segment on the heap starting at firstbyte **/
AAE_EXTERN_TOOL void aae_free(void * firstbyte);
/** realloc implementation **/
AAE_EXTERN_TOOL void* aae_realloc(void*firstbyte, aae_size_t numbytes);
#endif
