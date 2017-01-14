/** this allocator was designed for x86/_64 architecture **/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void*)0)
#endif
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#if defined(AAE_LINUX_PLATFORM)
#include <sys/types.h> /** size_t **/
#include <sys/mman.h> /** mmap, munmap **/
#include <unistd.h> /** sysconf **/
#define os_alloc(size) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0)
#define os_dealloc(start, size) munmap(start, size)
#elif defined(AAE_WINDOWS_PLATFORM)
#endif
#ifndef AAE_MAX_SMALL_BLOCK_SIZE
#define AAE_MAX_SMALL_BLOCK_SIZE 0x800
#endif
#ifndef AAE_GRANULARITY
#define AAE_GRANULARITY 0x8
#endif
#ifndef AAE_SUPERBLOCK_SIZE
#define AAE_SUPERBLOCK_SIZE 0x2000
#endif


inline static size_t get_page_size()
{
	static size_t m_page_size = 0; 
	if (!m_page_size)
		#if defined(AAE_LINUX_PLATFORM)
		m_page_size = sysconf(_SC_PAGESIZE);
		#elif defined(AAE_WINDOWS_PLATFORM)
		#endif
	return m_page_size;
}


static int32_t pointer_compare(const void* a, const void* b)
{
	return *(uintptr_t*)a - *(uintptr_t*)b;
}


#define MAXCREDITS (0x1<<0x6)
inline static uintptr_t get_active_pointer(uintptr_t active)
{
	return active & (~(MAXCREDITS-0x1));
}
inline static void set_active_credits(uintptr_t* active, uint32_t credits) 
{
	*active = get_active_pointer(*active);
	*active |= credits;
}
inline static uint32_t get_active_credits(uintptr_t active)
{
	return active & (MAXCREDITS-0x1);
}


/** organize a chunk of memory into a linked list **/
#define organize_linked_list(start_ptr, size, type)				\
type* ptr = start_ptr;								\
for (uint32_t it = 0; it < (size / sizeof(type)) - 1; ++it)			\
	ptr = ptr->m_next = (ptr + 1);						\
ptr->m_next = NULL;


/** first sizeof(void*) bytes of unreserved hold liniar id of next block **/
#define organize_list(start_ptr, count, stride)					\
void* ptr = start_ptr;								\
for (uint32_t it = 0; it < count - 1; ++it)					\
	*((uintptr_t*)(ptr + it * stride)) = (it + 1);


typedef struct __hp_record HPRecord;
struct __hp_record
{
	HPRecord* volatile m_next;
	void* m_hazard_ptr;
	uintptr_t m_is_active;
};
static HPRecord* volatile m_shared_hp_list_head = NULL;
static size_t volatile m_shared_hp_list_count = 0;


static HPRecord* volatile m_freelist_hp_head = NULL;
static HPRecord* allocate_hp()
{
	HPRecord* c_hp;
	for (;;)
	{
		c_hp = m_freelist_hp_head;
		if (c_hp)
		{
			HPRecord* c_next = c_hp->m_next;
			if (__sync_bool_compare_and_swap((void** volatile)&m_freelist_hp_head, c_hp, c_next)) break;
		}
		else
		{
			c_hp = os_alloc(get_page_size());
			/** oragnize hazard pointer superblock in a linked list **/
			organize_linked_list(c_hp, get_page_size(), HPRecord)

			__asm__ __volatile__ ("sfence" : : : "memory");
			if (__sync_bool_compare_and_swap((void** volatile)&m_freelist_hp_head, NULL, c_hp->m_next)) break;
			os_dealloc(c_hp, get_page_size());
		}
	}
	return c_hp;
}


static HPRecord* acquire_hp(HPRecord** volatile m_shared_list_head, size_t* volatile m_shared_list_count)
{
	HPRecord* c_hp = *m_shared_list_head;
	for (; c_hp; c_hp = c_hp->m_next)
	{
		if (c_hp->m_is_active || !__sync_bool_compare_and_swap(&c_hp->m_is_active, 0, 1))
			continue;
		return c_hp;
	}
	if (m_shared_list_count)
	{
		size_t old_length;
		do 
		{
			old_length = *m_shared_list_count;
      		} while (!__sync_bool_compare_and_swap((size_t* volatile)m_shared_list_count, old_length, old_length + 1));
      	}
      	c_hp = allocate_hp();
	c_hp->m_is_active = 1;
	c_hp->m_hazard_ptr = NULL;
	HPRecord* old_head;
	do
	{
		old_head = *m_shared_list_head;
		c_hp->m_next = old_head;
		__asm__ __volatile__ ("sfence" : : : "memory");
	}while(!__sync_bool_compare_and_swap((void** volatile)m_shared_list_head, old_head, c_hp));
	return c_hp;
}


static void release_hp(HPRecord* hp)
{
	hp->m_is_active = 0;
	hp->m_hazard_ptr = NULL;
}


#define large_block(descriptor) (uintptr_t)descriptor & 0x1
typedef enum
{
	ACTIVE,
	FULL,
	PARTIAL,
	EMPTY
}anchor_states;


typedef struct
{
	uint64_t m_available:10, m_count:10, m_state:2, m_tag:42;
}Anchor;


typedef struct __descriptor Descriptor;
typedef struct __processor_heap ProcessorHeap;


struct __descriptor 
{
	#ifdef AAE_32BIT_BUILD /** we need to keep 64 bit alignment **/
	uint32_t pad[9];
	#else
	uint32_t pad[6];
	#endif
	volatile Anchor m_anchor;
	Descriptor* volatile m_next;
	ProcessorHeap* m_heap;
	void* m_super_block;
	uint32_t m_block_size;
	uint32_t m_number_of_blocks;
};


typedef struct
{
	uint32_t m_block_size;
	uint32_t m_super_block_size;
}SizeClass;


struct __processor_heap
{
	volatile uintptr_t m_active;
	Descriptor* m_partial;
	SizeClass* m_size_class;
};


static SizeClass m_size_classes[AAE_MAX_SMALL_BLOCK_SIZE / AAE_GRANULARITY] = {};
__thread ProcessorHeap* m_heaps[AAE_MAX_SMALL_BLOCK_SIZE / AAE_GRANULARITY] = {};
__thread HPRecord* volatile m_private_descriptor_rlist = NULL;	
__thread size_t m_private_descriptor_rcount = 0;


static Descriptor* volatile m_freelist_descriptor_head = NULL;
static Descriptor* AllocateDescriptor()
{

	Descriptor *c_descriptor;
	for (;;)
	{
		/** we only break the infinite loop at the end so we can give the thread the chance to release hazard pointer **/
		uint32_t success = 0;
		HPRecord* c_hp = acquire_hp((HPRecord** volatile)&m_shared_hp_list_head, (size_t* volatile)&m_shared_hp_list_count);
		do /** set hazard pointer **/
		{
			c_descriptor = m_freelist_descriptor_head;
			c_hp->m_hazard_ptr = c_descriptor;
		}while(c_descriptor != m_freelist_descriptor_head);
		if (c_descriptor)
		{
			Descriptor* c_next = c_descriptor->m_next;
			success = __sync_bool_compare_and_swap((void** volatile)&m_freelist_descriptor_head, c_descriptor, c_next);
		}
		else
		{
			c_descriptor = os_alloc(get_page_size());
			organize_linked_list(c_descriptor, get_page_size(), Descriptor)
			/** organize descriptor super block in a linked list **/

			__asm__ __volatile__ ("sfence" : : : "memory");
			success = __sync_bool_compare_and_swap((void** volatile)&m_freelist_descriptor_head, NULL, c_descriptor->m_next);
			if (!success) /** some other thread got ahead of us and allocated a descriptor block - abort mission **/
				os_dealloc(c_descriptor, get_page_size());
		}
		release_hp(c_hp);
		if (success)
			break;
	}
	return c_descriptor;
}


static void descriptor_free_function(Descriptor* c_descriptor)
{
	Descriptor* old_head;
	do
	{
		old_head = m_freelist_descriptor_head;
		c_descriptor->m_next = old_head;
		__asm__ __volatile__ ("sfence" : : : "memory");
	}while(!__sync_bool_compare_and_swap((void** volatile)&m_freelist_descriptor_head, old_head, c_descriptor));
}


static void DeallocateDescriptor(Descriptor* c_descriptor)
{
	HPRecord* c_hp = acquire_hp((HPRecord** volatile)&m_private_descriptor_rlist, NULL);
	c_hp->m_hazard_ptr = c_descriptor;
	++m_private_descriptor_rcount;
	if (m_private_descriptor_rcount > 2 * m_shared_hp_list_count)
	{
		Descriptor** c_liniar_list = (Descriptor**)os_alloc(m_private_descriptor_rcount * sizeof(Descriptor*));
		uint32_t it = 0;
		c_hp = m_private_descriptor_rlist;
		for (; c_hp; c_hp = c_hp->m_next)
		{
			if (c_hp->m_is_active)
			{
				*(c_liniar_list + it) = c_hp->m_hazard_ptr;
				++it;
				release_hp(c_hp);
				--m_private_descriptor_rcount;
			}
		}
		qsort(c_liniar_list, it, sizeof(Descriptor*), pointer_compare);
		os_dealloc(c_liniar_list, it * sizeof(Descriptor*));
	}
}


static void UpdateActive(ProcessorHeap* c_heap, Descriptor* c_descriptor, uint32_t more_credits)
{
	uintptr_t new_active = (uintptr_t)c_descriptor;
	set_active_credits(&new_active, more_credits - 1);
	if (__sync_bool_compare_and_swap((void** volatile)&c_heap->m_active, NULL, new_active)) return;

	Anchor new_anchor, old_anchor;
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		new_anchor.m_count += more_credits;
		new_anchor.m_state = PARTIAL;
	}while(!__sync_bool_compare_and_swap((uint64_t* volatile)&c_descriptor->m_anchor, 
					    *(uint64_t* volatile)&old_anchor, 
					    *(uint64_t* volatile)&new_anchor));
}


static void* AllocateFromActiveSuperblock(ProcessorHeap* c_heap)
{
	Anchor new_anchor, old_anchor;
	uintptr_t new_active, old_active;
	uint32_t more_credits;
	void* address;
	do
	{
		new_active = old_active = c_heap->m_active;
		if (!old_active)
			return NULL;
		if (get_active_credits(old_active) == 0)
			new_active = 0;
		else
			--new_active;
	}while(!__sync_bool_compare_and_swap((void** volatile)&c_heap->m_active, old_active, new_active));
	Descriptor* c_descriptor = (Descriptor*)get_active_pointer(old_active);
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		address = c_descriptor->m_super_block + old_anchor.m_available * c_descriptor->m_block_size;
		uint32_t next = *((uintptr_t*)address);
		new_anchor.m_available = next;
		++new_anchor.m_tag;
		if (get_active_credits(old_active) == 0)
		{
			if (old_anchor.m_count == 0)
			{
				new_anchor.m_state = FULL;
			}
			else
			{
				more_credits = min(old_anchor.m_count, MAXCREDITS);
				new_anchor.m_count -= more_credits;
			}
		}

	}while(!__sync_bool_compare_and_swap((uint64_t* volatile)&c_descriptor->m_anchor, 
					    *(uint64_t* volatile)&old_anchor, 
					    *(uint64_t* volatile)&new_anchor));
	if (get_active_credits(old_active) == 0 && old_anchor.m_count > 0)
		UpdateActive(c_heap, c_descriptor, more_credits);
	*((Descriptor**)address) = c_descriptor;
	return (uintptr_t*)(address + sizeof(Descriptor*));

}


static void* AllocateFromNewSuperblock(ProcessorHeap* c_heap)
{
	Descriptor* c_descriptor = AllocateDescriptor();	
	c_descriptor->m_super_block = os_alloc(c_heap->m_size_class->m_super_block_size);
	c_descriptor->m_heap = c_heap;
	c_descriptor->m_block_size = c_heap->m_size_class->m_block_size;
	c_descriptor->m_number_of_blocks = c_heap->m_size_class->m_super_block_size / c_descriptor->m_block_size;


	organize_list(c_descriptor->m_super_block, c_descriptor->m_number_of_blocks, c_descriptor->m_block_size);
	uintptr_t new_active = (uintptr_t)c_descriptor;
	set_active_credits(&new_active, (min(c_descriptor->m_number_of_blocks - 1, MAXCREDITS) - 1)); /** n credits means n+1 available blocks **/
	c_descriptor->m_anchor.m_available = 1;
	c_descriptor->m_anchor.m_count = c_descriptor->m_number_of_blocks - get_active_credits(new_active) - 2;
	c_descriptor->m_anchor.m_state = ACTIVE; /** install it as the active superblock **/

	__asm__ __volatile__ ("sfence" : : : "memory");
	if (__sync_bool_compare_and_swap((void** volatile)&c_heap->m_active, NULL, new_active))
	{
		*((Descriptor**)c_descriptor->m_super_block) = c_descriptor;
		return (uintptr_t*)(c_descriptor->m_super_block + sizeof(Descriptor*));
	}
	else
	{
		os_dealloc(c_descriptor->m_super_block, c_descriptor->m_heap->m_size_class->m_super_block_size);
		DeallocateDescriptor(c_descriptor);
		return NULL;
	}

}


static ProcessorHeap* FindHeap(size_t size)
{
	if (size == 0) return NULL;
	/** We need to fit both the object and the descriptor in a single block **/
	size += sizeof(Descriptor*);
	if (size >= AAE_MAX_SMALL_BLOCK_SIZE) return NULL; /** large block **/

	static uint32_t c_has_initialized_size_classes = 0;
	if (!c_has_initialized_size_classes)
	{
		for (uint32_t it = 0; it < AAE_MAX_SMALL_BLOCK_SIZE / AAE_GRANULARITY; ++it)
		{
			m_size_classes[it].m_block_size = (it + 1) * AAE_GRANULARITY;
			m_size_classes[it].m_super_block_size = AAE_SUPERBLOCK_SIZE;
		}
		c_has_initialized_size_classes = 1;
	}
	--size; /** an allocation of n * AAE_GRANULARITY should fit in the (n - 1)th size class **/
	ProcessorHeap* c_heap = m_heaps[size / AAE_GRANULARITY];
	if (c_heap == NULL) 
	{
		c_heap = (ProcessorHeap*)allocate_hp(); /** we used hp allocator because sizeof(HPRecord) = sizeof(ProcessorHeap) **/
		c_heap->m_active = 0;
		c_heap->m_size_class = &m_size_classes[size / AAE_GRANULARITY];
		m_heaps[size / AAE_GRANULARITY] = c_heap;
	}
	return c_heap;
}


void* aae_malloc(size_t size)
{

	ProcessorHeap *c_heap = FindHeap(size);
	void* address;
	if (!c_heap) 
	{
		size_t header_size = (sizeof(Descriptor*)) << 1;
		address = os_alloc(size + header_size);
		*((uintptr_t*)address) = size; /** encode the size of the large block **/
		*((uintptr_t*)(address + (header_size >> 1))) |= 0x1;
		return (uintptr_t*)(address + header_size);
	}
	while (1)
	{
		address = AllocateFromActiveSuperblock(c_heap);
		if (address) return address;
		address = AllocateFromNewSuperblock(c_heap);	
		if (address) return address;
	}
}


void aae_free(void* start)
{
	if (!start) return;
	start = (uintptr_t*)(start - sizeof(Descriptor*));
	Descriptor* c_descriptor = *((Descriptor**)start);
	if (large_block(c_descriptor))
	{
		void* header_start = ((uintptr_t*)(start - sizeof(Descriptor*)));
		size_t size = *((uintptr_t*)header_start);
		os_dealloc(header_start, size);
		return;
	}
	void* c_super_block = c_descriptor->m_super_block;
	Anchor new_anchor, old_anchor;
	ProcessorHeap* c_heap;
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		*((uintptr_t*)start) = old_anchor.m_available;
		new_anchor.m_available = (uintptr_t)(start - c_super_block) / c_descriptor->m_block_size;
		if (old_anchor.m_state == FULL)
			new_anchor.m_state = PARTIAL;
		if (old_anchor.m_count == c_descriptor->m_number_of_blocks - 1)
		{
			c_heap = c_descriptor->m_heap;
			new_anchor.m_state = EMPTY;	
		}
		else
		{
			new_anchor.m_count++;
		}
		__asm__ __volatile__ ("sfence" : : : "memory");
	}while(!__sync_bool_compare_and_swap((uint64_t* volatile)&c_descriptor->m_anchor, 
					    *(uint64_t* volatile)&old_anchor, 
					    *(uint64_t* volatile)&new_anchor));

}