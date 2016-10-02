#ifndef AAE_MEMORY_ALLOCATOR_H
#define AAE_MEMORY_ALLOCATOR_H



#include "aae_types.h"



#ifdef __GNUC__
extern "C++"
{
	inline void* operator new(aae_size_t, void* __p) { return __p; }
	inline void* operator new[](aae_size_t, void* __p) { return __p; }
}
#endif




template <typename T, class Arena>
static inline void aae_delete(T * object, Arena & arena)
{
	object->~T();
	arena.Free(object);
}



//allocate and deallocate using provided memory arena
#define AAE_NEW(type, arena)  new (arena.Allocate(sizeof(type)))type
#define AAE_DELETE(object, arena) aae_delete(object, arena)




#endif
