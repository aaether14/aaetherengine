#ifndef AAE_ALLOCATOR_H
#define AAE_ALLOCATOR_H


#include <stdint.h>
#include "aae_defines.h"


#ifdef __cplusplus
AAE_EXTERN_TOOL {
#endif




void * aae_malloc(uint32_t numbytes);
void aae_free(void * firstbyte);
uint32_t aae_allocated_memory();



#ifdef __cplusplus
}
#endif




#endif
