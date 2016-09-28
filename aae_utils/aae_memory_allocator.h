/**
*aae_memory_allocator.h - Stefan Cristian Dinescu a.k.a Aaether - 22.08.2016
*/


#ifndef AAE_MEMORY_ALLOCATOR_H
#define AAE_MEMORY_ALLOCATOR_H



#include "aae_types.h"
#include "aae_allocator.h"
#include <new>



struct aae_allocator
{
	
	inline void * Allocate(aae_size_t size)
	{
		return aae_malloc(size); 
	}

	inline void Free(void * start)
	{
		aae_free(start);
	}

};





template <typename T, class Arena>
void aae_delete(T * object, Arena & arena)
{
	object->~T();
	arena.Free(object);
}


//allocate and deallocate using provided memory arena
#define AAE_NEW(type, arena)  new (arena.Allocate(sizeof(type)))type
#define AAE_DELETE(object, arena) aae_delete(object, arena)




#endif
