#ifndef AAE_MEMORY_ALLOCATOR_H
#define AAE_MEMORY_ALLOCATOR_H


#include "aae_types.h"


#ifdef AAE_LINUX_PLATFORM
extern "C++"
{
	inline void* operator new(aae_size_t, void* __p) { return __p; }
	inline void* operator new[](aae_size_t, void* __p) { return __p; }
}
#endif


template <typename T, class Arena>
static inline void aae_delete(T* object, Arena& arena)
{
	object->~T();
	arena.Free(object);
}


template <typename T, class Arena>
static inline void aae_delete_array(T* array, Arena& arena)
{
	aae_size_t array_size = *(aae_size_t*)((uintptr_t)array - sizeof(aae_size_t));
	for (i32 it = array_size - 1; it >= 0; --it)
		array[it].~T();
	arena.Free((T*)((uintptr_t)array - sizeof(aae_size_t)));
}


template <typename T, class Arena>
static inline T* aae_new_array(Arena& arena, aae_size_t n_instances)
{
	void* address = arena.Allocate(sizeof(T) * n_instances + sizeof(aae_size_t));
	*((aae_size_t*)address) = n_instances;
	for (i32 it = 0; it < n_instances; ++it)
		new((T*)address + it) T;
	return (T*)((uintptr_t)address + sizeof(aae_size_t));
}





/** allocate and deallocate using provided memory arena **/
#define AAE_NEW(type, arena) new(arena.Allocate(sizeof(type)))type
#define AAE_DELETE(object, arena) aae_delete(object, arena)
#define AAE_NEW_ARRAY(type, n, arena) aae_new_array<type>(arena, n)
#define AAE_DELETE_ARRAY(array, arena) aae_delete_array(array, arena)


#endif