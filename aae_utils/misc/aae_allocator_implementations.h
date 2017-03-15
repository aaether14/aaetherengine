#ifndef AAE_ALLOCATOR_IMPLEMENTATIONS_H
#define AAE_ALLOCATOR_IMPLEMENTATIONS_H
#define AAE_PROVIDE_ALLOCATOR_DEFINITIONS
#include "aae_memory.h"
#include "../lib/aae_malloc.h"
namespace aae
{
	template <
                typename                                                AllocationPolicy, 
                typename                                                ThreadPolicy, 
                typename                                                BoundsCheckingPolicy, 
                typename                                                MemoryTrackingPolicy, 
                typename                                                MemoryTaggingPolicy
                >
	class MemoryArena : public AAEAllocatorInterface
	{

		AllocationPolicy                                        m_allocator;
		ThreadPolicy                                            m_thread_guard;
		BoundsCheckingPolicy                                    m_bounds_checker;
		MemoryTrackingPolicy                                    m_memory_tracker;
		MemoryTaggingPolicy                                     m_memory_tagger;

		public:
		void* Allocate(aae_size_t allocation_size) noexcept
		{

			if (!allocation_size)                           return nullptr;
			void* allocated_memory =			nullptr;
			try
			{
				m_thread_guard.enter();
				aae_size_t adjusted_size = 			BoundsCheckingPolicy::SIZE_FRONT + 
                                	                                        BoundsCheckingPolicy::SIZE_BACK + 
                                                	                        sizeof(void*) +
										allocation_size;


				allocated_memory =				m_allocator.Allocate(adjusted_size);
                        	if (!allocated_memory)                          return nullptr;
				*static_cast<aae_size_t*>(allocated_memory) =	adjusted_size;


				m_bounds_checker.guard_front(reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(allocated_memory) + 
										sizeof(void*)));
				m_memory_tagger.tag_allocation(reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(allocated_memory) + 
										BoundsCheckingPolicy::SIZE_FRONT + 
										sizeof(void*)), 
										allocation_size);
				m_bounds_checker.guard_back(reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(allocated_memory) + 
										BoundsCheckingPolicy::SIZE_FRONT + 
										sizeof(void*) +  
										allocation_size));
				m_memory_tracker.on_allocation(allocated_memory, adjusted_size);
				m_thread_guard.leave();


				return reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(allocated_memory) + 
										BoundsCheckingPolicy::SIZE_FRONT + 
										sizeof(void*));
			}
			catch(...)
			{
				if (allocated_memory) 				m_allocator.Deallocate(allocated_memory);
				return nullptr;
			}
		}
		void Deallocate(void* start) noexcept
		{
                        if (!start) 					return;
			try
			{
				m_thread_guard.enter();
				void* actual_start = reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(start) - 
										BoundsCheckingPolicy::SIZE_FRONT - 
										sizeof(void*));
				aae_size_t allocation_size = *(static_cast<aae_size_t*>(actual_start));
				m_bounds_checker.check_front(reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(start) - 
										BoundsCheckingPolicy::SIZE_FRONT));
				m_bounds_checker.check_back(reinterpret_cast<void*>(reinterpret_cast<aae_size_t>(actual_start) + 
										allocation_size -
										BoundsCheckingPolicy::SIZE_BACK));
				m_memory_tracker.on_deallocation(actual_start);
				m_memory_tagger.tag_deallocation(actual_start, allocation_size);
				m_allocator.Deallocate(actual_start); 
				m_thread_guard.leave();
			}
			catch(...)
			{
				return;
			}
		}
	};
	class MallocAllocationPolicy : public AAEAllocatorInterface
	{
		public:
		void* Allocate(aae_size_t size) noexcept { return aae_malloc(size); }
		void Deallocate(void* start) noexcept { aae_free(start); }
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
        static MemoryArena
								<MallocAllocationPolicy, 
								SingleThreadedPolicy, 
								NoBoundsCheckingPolicy, 
								NoMemoryTrackingPolicy, 
								NoMemoryTaggingPolicy
								> DefaultAllocator;
        AAEAllocatorInterface& mallocator = 			DefaultAllocator;
	static MallocAllocationPolicy 				DefaultAllocatorRelease;
	AAEAllocatorInterface& mallocator_release = 		DefaultAllocatorRelease;
}
#endif