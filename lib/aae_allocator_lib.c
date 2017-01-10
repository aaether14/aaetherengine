#include <stdint.h>


#ifndef NULL
#define NULL ((void*)0)
#endif
#ifdef AAE_LINUX_PLATFORM
#include <sys/types.h> /** size_t **/
#include <sys/mman.h> /** mmap, munmap **/
#include <unistd.h> /** sysconf **/
#endif


static size_t m_page_size = 0; 


/** organize a chunk of memory into a linked list **/
#define organize_linked_list(start_ptr, size, type)				\
type* ptr = start_ptr;								\
for (int32_t it = 0; it < (size / sizeof(type)) - 1; ++it)			\
	ptr = ptr->m_next = (ptr + 1);						\
ptr->m_next = NULL;


typedef struct __hp_record HPRecord;
struct __hp_record
{
	HPRecord* volatile m_next;
	void* m_hazard_ptr;
	volatile size_t m_is_active;
};
static HPRecord* volatile m_shared_hp_list_head = NULL;
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
			if (!m_page_size) /** get page size if you haven't done it yet **/
				#ifdef AAE_LINUX_PLATFORM
				m_page_size = sysconf(_SC_PAGESIZE);
				#endif
			c_hp = mmap(NULL, m_page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			/** oragnize hazard pointer superblock in a linked list **/
			organize_linked_list(c_hp, m_page_size, HPRecord)

			if (__sync_bool_compare_and_swap((void** volatile)&m_freelist_hp_head, NULL, c_hp->m_next)) break;
			munmap(c_hp, m_page_size);
		}
	}
	return c_hp;
}


static HPRecord* acquire_hp()
{
	HPRecord* c_hp = m_shared_hp_list_head;
	for (; c_hp; c_hp = c_hp->m_next)
	{
		if (c_hp->m_is_active || !__sync_bool_compare_and_swap(&c_hp->m_is_active, 0, 1))
			continue;
		return c_hp;
	}
	c_hp = allocate_hp();
	c_hp->m_is_active = 1;
	c_hp->m_hazard_ptr = NULL;

	HPRecord* old_head;
	do
	{
		old_head = m_shared_hp_list_head;
		c_hp->m_next = old_head;
	}while(!__sync_bool_compare_and_swap((void** volatile)&m_shared_hp_list_head, old_head, c_hp));
	return c_hp;
}


static void release_hp(HPRecord* hp)
{
	hp->m_is_active = 0;
	hp->m_hazard_ptr = NULL;
}


#define MASKCREDITS(active) ((Descriptor*)active.m_ptr)
#define SUPERBLOCKSIZE 65536
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


#ifdef AAE_32BIT_BUILD /** addresses have 32 bits **/
typedef struct
{
	uint32_t m_ptr:24, m_credits:6;
}Active;
#else
#ifdef AAE_64BIT_BUILD /** addresses have 64 bits **/
typedef struct
{
	uint64_t m_ptr:56, m_credits:6;
}Active;
#else
#error "AAE_32BIT_BUILD or AAE_64BIT_BUILD should be defined!"
#endif
#endif


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
	Active m_active;
	Descriptor* m_partial;
	SizeClass* m_size_class;
};


static Descriptor* volatile m_descriptor_list_head = NULL;
static SizeClass m_size_classes[] = {{8, SUPERBLOCKSIZE}, {32, SUPERBLOCKSIZE},
				     {256, SUPERBLOCKSIZE}, {1024, SUPERBLOCKSIZE}};
__thread ProcessorHeap* m_heaps[sizeof(m_size_classes) / sizeof(SizeClass)] = {};


static Descriptor* AllocateDescriptor()
{

	Descriptor *c_descriptor = NULL;
	for (;;)
	{
		/** we only break the infinite loop at the end so we can give the thread the chance to release hazard pointer **/
		int32_t success = 0;
		HPRecord* c_hp = acquire_hp();
		do /** set hazard pointer **/
		{
			c_descriptor = m_descriptor_list_head;
			c_hp->m_hazard_ptr = c_descriptor;
		}while(c_descriptor != m_descriptor_list_head);
		if (c_descriptor)
		{
			Descriptor* c_next = c_descriptor->m_next;
			success = __sync_bool_compare_and_swap((void** volatile)&m_descriptor_list_head, c_descriptor, c_next);
		}
		else
		{
			if (!m_page_size) /** get page size if you haven't done it yet **/
				#ifdef AAE_LINUX_PLATFORM
				m_page_size = sysconf(_SC_PAGESIZE);
				#endif
			c_descriptor = mmap(NULL, m_page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
			organize_linked_list(c_descriptor, m_page_size, Descriptor)
			/** organize descriptor super block in a linked list **/

			success = __sync_bool_compare_and_swap((void** volatile)&m_descriptor_list_head, NULL, c_descriptor->m_next);
			if (!success) /** some other thread got ahead of us and allocated a descriptor block - abort mission **/
				munmap(c_descriptor, m_page_size);
		}
		release_hp(c_hp);
		if (success)
			break;
	}
	return c_descriptor;
}


static void DeallocateDescriptor(Descriptor* c_descriptor)
{
	Descriptor* old_head;
	do
	{
		old_head = m_descriptor_list_head;
		c_descriptor->m_next = old_head;
	}while(!__sync_bool_compare_and_swap((void** volatile)&m_descriptor_list_head, old_head, c_descriptor));
}


static ProcessorHeap* FindHeap(size_t size)
{

	ProcessorHeap* c_heap; 
	/** We need to fit both the object and the descriptor in a single block **/
	size += sizeof(size_t);
	if (size > 1024) /** large block **/
		return NULL;
  
	c_heap = m_heaps[1];
	if (c_heap == NULL) 
	{
		c_heap = mmap(NULL, sizeof(ProcessorHeap), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		*((size_t*)&(c_heap->m_active)) = 0;
		c_heap->m_size_class = &m_size_classes[1];
		m_heaps[1] = c_heap;
	}
	return c_heap;
}


void* aae_malloc(size_t size)
{

	ProcessorHeap *c_heap = FindHeap(size);
	AllocateDescriptor();
	void* address;
	if (!c_heap) 
	{
		address = NULL;
		return address;
	}
	while (1)
	{
		return NULL;
	}
}


void aae_free(void* start)
{
	if (!start) return;
}