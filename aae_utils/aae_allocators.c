#include "aae_allocators.h"



void * aae_allocator::Allocate(aae_size_t size)
{
	return aae_malloc(size);
}



void aae_allocator::Free(void * start)
{
	aae_free(start);
}



aae_allocator aae_mallocator;
