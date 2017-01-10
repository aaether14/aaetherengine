#ifndef AAE_ALLOCATORS_H
#define AAE_ALLOCATORS_H



#include "aae_lib_allocator.h"
struct aae_allocator
{
	
	void* Allocate(aae_size_t size);
	void Free(void* start);

};
#endif
