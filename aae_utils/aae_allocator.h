#ifndef AAE_ALLOCATOR_H
#define AAE_ALLOCATOR_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif




void * aae_malloc(uint32_t numbytes);
void aae_free(void * firstbyte);
uint32_t aae_memory_used();



#ifdef __cplusplus
}
#endif




#endif
