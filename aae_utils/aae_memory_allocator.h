/**
*aae_memory_allocator.h - Stefan Cristian Dinescu a.k.a Aaether - 22.08.2016
*/


#ifndef AAE_MEMORY_ALLOCATOR_H
#define AAE_MEMORY_ALLOCATOR_H



#include "aae_types.h"
#include <malloc.h>
#include <new>



struct aae_allocator
{
	
	inline void * allocate(aae_size_t size)
	{
		return malloc(size); 
	}

	inline void free(void * start)
	{
		free(start);
	}

};



// simple mallocator
extern aae_allocator mallocator;



//allocate using provided memory arena
#define AAE_NEW(type, arena)  new (arena.allocate(sizeof(type)))type



#endif
