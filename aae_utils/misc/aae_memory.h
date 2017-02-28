#ifndef AAE_ALLOCATORS_H
#define AAE_ALLOCATORS_H
#include "../lib/aae_lib_allocator.h"


class AAEAllocatorInterface
{
	public:
	virtual ~AAEAllocatorInterface() {}
	virtual void* Allocate(aae_size_t size) = 0;
	virtual void Deallocate(void* start) = 0;
};


#ifndef AAE_PROVIDE_MEMORY_ARENA_DEFINITION
#include "aae_new.h"
extern AAEAllocatorInterface* aae_mallocator;
#endif
#ifdef AAE_PROVIDE_MEMORY_ARENA_DEFINITION
template <class AllocationPolicy, class ThreadPolicy, class BoundsCheckingPolicy, class MemoryTrackingPolicy, class MemoryTaggingPolicy>
class MemoryArena : public AAEAllocatorInterface
{


	AllocationPolicy m_allocator;
	ThreadPolicy m_thread_guard;
	BoundsCheckingPolicy m_bounds_checker;
	MemoryTrackingPolicy m_memory_tracker;
	MemoryTaggingPolicy m_memory_tagger;


	public:
 

	void* Allocate(aae_size_t allocation_size)
	{
		m_thread_guard.enter();
		aae_size_t adjusted_size = allocation_size + BoundsCheckingPolicy::SIZE_FRONT + BoundsCheckingPolicy::SIZE_BACK + sizeof(void*);
		void* allocated_memory = m_allocator.Allocate(adjusted_size);
		m_bounds_checker.guard_front(static_cast<void*>(static_cast<byte_ptr>(allocated_memory) + sizeof(void*)));
		m_memory_tagger.tag_allocation(static_cast<void*>(static_cast<byte_ptr>(allocated_memory) + BoundsCheckingPolicy::SIZE_FRONT + sizeof(void*)), allocation_size);
		m_bounds_checker.guard_back(static_cast<void*>(static_cast<byte_ptr>(allocated_memory) + BoundsCheckingPolicy::SIZE_FRONT + sizeof(void*) + allocation_size));
		m_memory_tracker.on_allocation(allocated_memory, adjusted_size);
		m_thread_guard.leave();
        	return static_cast<void*>(static_cast<byte_ptr>(allocated_memory) + BoundsCheckingPolicy::SIZE_FRONT + sizeof(void*));
        }
 


	void Deallocate(void* start)
	{
		m_thread_guard.enter();
		void* actual_start = static_cast<void*>(static_cast<byte_ptr>(start) - BoundsCheckingPolicy::SIZE_FRONT - sizeof(void*));
		aae_size_t allocation_size = *(static_cast<aae_size_t*>(actual_start));
		m_bounds_checker.check_front(static_cast<void*>(static_cast<byte_ptr>(start) - BoundsCheckingPolicy::SIZE_FRONT));
		m_bounds_checker.check_back(static_cast<void*>(static_cast<byte_ptr>(start) - BoundsCheckingPolicy::SIZE_BACK + allocation_size));
		m_memory_tracker.on_deallocation(actual_start);
		m_memory_tagger.tag_deallocation(actual_start, allocation_size);
		m_allocator.Deallocate(actual_start); 
		m_thread_guard.leave();
	}


};


class MallocAllocationPolicy
{
	public:
	void* Allocate(aae_size_t size) const { return aae_malloc(size); }
	void Deallocate(void* start) const { aae_free(start); }
};
class NoBoundsCheckingPolicy
{
	public:
	static const aae_size_t SIZE_FRONT = 0;
	static const aae_size_t SIZE_BACK = 0;
	inline void guard_back(void*) const {}
	inline void guard_front(void*) const {}
	inline void check_back(void*) const {}
	inline void check_front(void*) const {}
};
class NoMemoryTrackingPolicy
{
	public:
	inline void on_allocation(void*, aae_size_t) const {}
	inline void on_deallocation(void*) const {}
};
class NoMemoryTaggingPolicy
{
	public:
	inline void tag_allocation(void*, aae_size_t) const {}
	inline void tag_deallocation(void*, aae_size_t) const {}
};
class SingleThreadedPolicy
{
	public:
	inline void enter() const {}
	inline void leave() const {}
};
typedef MemoryArena<MallocAllocationPolicy, SingleThreadedPolicy, NoBoundsCheckingPolicy, NoMemoryTrackingPolicy, NoMemoryTaggingPolicy> DefaultMemoryArenaType;
#undef AAE_PROVIDE_MEMORY_ARENA_DEFINITION
#endif
#endif
