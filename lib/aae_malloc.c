#include <stdint.h>
#include <stdlib.h> /** qsort **/
#include <string.h> /** memcpy **/


#if !defined(AAE_32BIT_BUILD) && !defined(AAE_64BIT_BUILD)
#error "One of AAE_32BIT_BUILD or AAE_64BIT_BUILD MUST be defined!"
#endif
#define AAE_NULL ((void*)0)
#define aae_min(a,b) ((a) < (b) ? (a) : (b))
#define aae_max(a,b) ((a) > (b) ? (a) : (b))
#if defined(AAE_LINUX_PLATFORM)
#include <sys/types.h> /** size_t **/
#include <sys/mman.h> /** mmap, munmap **/
#include <unistd.h> /** sysconf **/
#define MEMORY_FENCE __asm__ __volatile__("mfence" : : : "memory")
#define STORE_FENCE __asm__ __volatile__("sfence" : : : "memory")
#define LOAD_FENCE __asm__ __volatile__("lfence" : : : "memory")
#define thread_local __thread /** assuming gcc **/
#define CompareAndSwap(adress, expected, new) __sync_bool_compare_and_swap((adress), (expected), (new))
int32_t sched_getcpu(void);
static thread_local int32_t m_allocation_error_flag = 0;
static inline int32_t os_dealloc(void* start, size_t size) { return munmap(start, size); }
static inline void* os_alloc(size_t size)
{
	void* result = mmap(AAE_NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (result == MAP_FAILED) { m_allocation_error_flag = -1; return AAE_NULL; }
	return result;
}
inline static size_t get_page_size()
{
	static size_t m_page_size = 0;
	if (!m_page_size) m_page_size = sysconf(_SC_PAGESIZE);
	return m_page_size;
}
inline static int32_t get_current_cpu()
{
	return sched_getcpu();
}
#elif defined(AAE_WINDOWS_PLATFORM)
#include <windows.h>
#define thread_local __declspec(thread) /** assuming msvc **/
static inline size_t get_page_size()
{
	static size_t m_page_size = 0;
	if (m_page_size) { SYSTEM_INFO c_system_info; GetSystemInfo(&c_system_info); m_page_size = c_system_info.dwPageSize; }
	return m_page_size;
}
inline static int32_t get_current_cpu()
{
	return GetCurrentProcessorNumber();
}
#else
#error "One of AAE_LINUX_PLATFORM or AAE_WINDOWS_PLATFORM MUST be defined!"
#endif
#ifndef AAE_NUMBER_OF_SIZE_CLASSES
#define AAE_NUMBER_OF_SIZE_CLASSES 256 /** can't be bigger than 512 **/
#endif
#ifndef AAE_GRANULARITY
#define AAE_GRANULARITY 8
#endif
#ifndef AAE_SUPERBLOCK_SIZE
#define AAE_SUPERBLOCK_SIZE 16384
#endif
#ifndef AAE_NUMBER_OF_CPUS
#define AAE_NUMBER_OF_CPUS 1
#endif
#ifdef AAE_USE_DEFAULT_NAMES
#define aae_malloc malloc
#define aae_free free
#define aae_realloc realloc
#define aae_calloc calloc
#endif


static int32_t pointer_compare(const void* a, const void* b)
{
	return *(uintptr_t*)a - *(uintptr_t*)b;
}
static uint32_t pointer_binary_search(uintptr_t* array, int32_t left, int32_t right, uintptr_t value)
{
	while (left <= right)
	{
		uint32_t middle = (int32_t)((int64_t)(left + right) >> 1);
		if (*(array + middle) == value)
			return 1;
		if (*(array + middle) < value)
			left = middle + 1;
		else
			right = middle - 1;
	}
	return 0;
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
ptr->m_next = AAE_NULL;


/** first sizeof(void*) bytes of unreserved hold liniar id of next block **/
#define organize_list(start_ptr, count, stride)					\
void* ptr = start_ptr;								\
for (uint32_t it = 0; it < count - 1; ++it)					\
	*((uintptr_t*)(ptr + it * stride)) = (it + 1);


#define create_allocator(type, freelist) /** requires volatile freelist **/										\
static type* Allocate##type()																\
{																			\
	type *c_node;																	\
	HPRecord* c_hp = acquire_hp(&m_shared_hp_list);													\
	if (!c_hp) return AAE_NULL;															\
	while(1)																	\
	{																		\
		uint32_t success = 0;															\
		c_node = freelist;															\
		c_hp->m_hazard_ptr = c_node;														\
		MEMORY_FENCE;																\
		if (freelist != c_node) continue;													\
		if (c_node)																\
		{																	\
			type* c_next = c_node->m_next;													\
			success = CompareAndSwap((void* volatile*)&freelist, c_node, c_next);								\
		}																	\
		else																	\
		{																	\
			c_node = os_alloc(get_page_size());												\
			if (!c_node) return AAE_NULL;													\
			organize_linked_list(c_node, get_page_size(), type)										\
			/** organize descriptor super block in a linked list **/									\
																			\
			STORE_FENCE;															\
			success = CompareAndSwap((void* volatile*)&freelist, AAE_NULL, c_node->m_next);							\
			if (!success) /** some other thread got ahead of us and allocated a descriptor block - abort mission **/			\
				os_dealloc(c_node, get_page_size());											\
		}																	\
		release_hp(c_hp);															\
		if (success)																\
			break;																\
	}																		\
	return c_node;																	\
}


#define create_deallocator(type, freelist) 														\
static void Deallocate##type(void* c_node)														\
{																			\
	type* old_head;																	\
	do 																		\
	{																		\
		old_head = freelist;															\
		((type*)c_node)->m_next = old_head;													\
		STORE_FENCE;																\
	}while(!CompareAndSwap(&freelist, old_head, c_node));												\
}


typedef struct __hp_record HPRecord;
struct __hp_record /** hazard pointer structure **/
{
	HPRecord* volatile m_next;
	void* volatile m_hazard_ptr;
	uintptr_t volatile m_is_active;
};
typedef struct
{
	HPRecord* volatile m_hp_head;
	size_t volatile m_hp_count;
}HPRecordList;
static HPRecordList m_shared_hp_list = {AAE_NULL, 0};


static HPRecord* volatile m_freelist_hp_head = AAE_NULL;
static HPRecord* allocate_hp()
{
	HPRecord* c_hp;
	while (1)
	{
		c_hp = m_freelist_hp_head;
		if (c_hp)
		{
			HPRecord* c_next = c_hp->m_next;
			if (CompareAndSwap((void* volatile*)&m_freelist_hp_head, c_hp, c_next)) break;
		}
		else
		{
			c_hp = os_alloc(get_page_size());
			if (!c_hp) return AAE_NULL;
			/** oragnize hazard pointer superblock in a linked list **/
			organize_linked_list(c_hp, get_page_size(), HPRecord)

			STORE_FENCE;
			if (CompareAndSwap((void* volatile*)&m_freelist_hp_head, AAE_NULL, c_hp->m_next)) break;
			os_dealloc(c_hp, get_page_size());
		}
	}
	return c_hp;
}


static HPRecord* acquire_hp(HPRecordList* m_list)
{
	HPRecord* c_hp = m_list->m_hp_head;
	for (; c_hp; c_hp = c_hp->m_next)
	{
		if (c_hp->m_is_active || !CompareAndSwap(&c_hp->m_is_active, 0, 1))
			continue;
		return c_hp;
	}

	size_t old_length;
	do
	{
		old_length = m_list->m_hp_count;
	}while(!CompareAndSwap((size_t volatile*)&m_list->m_hp_count, old_length, old_length + 1));

      	c_hp = allocate_hp();
      	if (!c_hp) return AAE_NULL;
	c_hp->m_is_active = 1;
	c_hp->m_hazard_ptr = AAE_NULL;
	HPRecord* old_head;
	do
	{
		old_head = m_list->m_hp_head;
		c_hp->m_next = old_head;
		STORE_FENCE;
	}while(!CompareAndSwap((void* volatile*)&m_list->m_hp_head, old_head, c_hp));
	return c_hp;
}


static void release_hp(HPRecord* m_hp)
{
	m_hp->m_is_active = 0;
	m_hp->m_hazard_ptr = AAE_NULL;
}


static void InsertRetiredNode(HPRecordList *c_retired_list, void* c_ptr)
{
	size_t c_retired_list_actual_count = c_retired_list->m_hp_count + 1;
	HPRecord* c_hp = acquire_hp(c_retired_list);
	if (!c_hp) return;
	c_hp->m_hazard_ptr = c_ptr;
	c_retired_list->m_hp_count = c_retired_list_actual_count;
}


static void ScanRetiredNodes(HPRecordList *c_retired_list, HPRecordList* c_shared_list, void (*free_function)(void*))
{
	if (c_retired_list->m_hp_count > 2 * c_shared_list->m_hp_count)
	{
		uint32_t it = 0;
		HPRecord* c_shared_hp_list_head = c_shared_list->m_hp_head;
		size_t c_shared_hp_list_count = c_shared_list->m_hp_count; /** this is at most no. of shared hp's + 1 **/
		void** c_liniar_list = (void**)os_alloc(c_shared_hp_list_count * sizeof(void*));
		if (!c_liniar_list) return;
		while (c_shared_hp_list_head)
		{
			if (c_shared_hp_list_head->m_is_active)
				*(c_liniar_list + it++) = c_shared_hp_list_head->m_hazard_ptr;
			c_shared_hp_list_head = c_shared_hp_list_head->m_next;
		}
		qsort(c_liniar_list, it, sizeof(void*), &pointer_compare);
		HPRecord* c_retired_list_head = c_retired_list->m_hp_head;
		while (c_retired_list_head)
		{
			if (c_retired_list_head->m_hazard_ptr)
				if (!pointer_binary_search((uintptr_t*)c_liniar_list, 0, it - 1, (uintptr_t)c_retired_list_head->m_hazard_ptr))
				{
					free_function(c_retired_list_head->m_hazard_ptr);
					release_hp(c_retired_list_head);
					--c_retired_list->m_hp_count;
				}
			c_retired_list_head = c_retired_list_head->m_next;
		}
		os_dealloc(c_liniar_list, c_shared_hp_list_count * sizeof(void*));
	}
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
	uint64_t m_available:15, m_count:15, m_state:2, m_tag:32;
}Anchor;


typedef struct __descriptor Descriptor;
typedef struct __processor_heap ProcessorHeap;


struct __descriptor
{
	#ifdef AAE_32BIT_BUILD /** we need to keep 64 byte alignment to encode credits into active blocks **/
	uint32_t pad[9];
	#else
	uint32_t pad[6];
	#endif
	Anchor volatile m_anchor;
	Descriptor* volatile m_next;
	ProcessorHeap* m_heap;
	void* m_super_block;
	uint32_t m_block_size;
	uint32_t m_number_of_blocks;
};


struct __processor_heap
{
	uintptr_t m_size_class;
	uintptr_t volatile m_active;
	Descriptor* volatile m_partial;
};
#include "aae_pp_enum.h"
#define ProcessorHeapStaticInitialize(n) { ((n + 1) * AAE_GRANULARITY), 0, AAE_NULL }
#define PerCPUProcessorHeapStaticInitialize { AAE_ENUM(AAE_NUMBER_OF_SIZE_CLASSES)(ProcessorHeapStaticInitialize) }
#define PerCPUProcessorHeapStaticInitializeMacro(n) PerCPUProcessorHeapStaticInitialize
static ProcessorHeap m_heaps[AAE_NUMBER_OF_CPUS][AAE_NUMBER_OF_SIZE_CLASSES] = { AAE_ENUM(AAE_NUMBER_OF_CPUS)(PerCPUProcessorHeapStaticInitializeMacro) };


static thread_local HPRecordList m_retired_descriptor_list = {AAE_NULL, 0};
static Descriptor* volatile m_freelist_descriptor_head = AAE_NULL;
create_allocator(Descriptor, m_freelist_descriptor_head);
create_deallocator(Descriptor, m_freelist_descriptor_head);


typedef struct __queue_node QueueNode;
struct __queue_node
{
	QueueNode* volatile m_next;
	void* m_data;
}*volatile m_freelist_queue_head = AAE_NULL;
typedef struct
{
	QueueNode* volatile m_head;
	QueueNode* volatile m_tail;
}Queue;
static Queue m_queues[AAE_NUMBER_OF_SIZE_CLASSES] = {}; /** one for each size class **/
static thread_local HPRecordList m_retired_queue_list = {AAE_NULL, 0};
create_allocator(QueueNode, m_freelist_queue_head);
create_deallocator(QueueNode, m_freelist_queue_head);


static void QueueLazyInitialization(Queue* m_queue)
{
	QueueNode* c_node;
	while (m_queue->m_head == AAE_NULL)
	{
		c_node = AllocateQueueNode();
		if (!c_node) return;
		c_node->m_next = AAE_NULL;
		STORE_FENCE;
		if (CompareAndSwap((void* volatile*)&m_queue->m_head, AAE_NULL, c_node)) break;
		InsertRetiredNode(&m_retired_queue_list, c_node);
		ScanRetiredNodes(&m_retired_queue_list, &m_shared_hp_list, DeallocateQueueNode);
	}
	while (m_queue->m_tail == AAE_NULL)
		if (CompareAndSwap((void* volatile*)&m_queue->m_tail, AAE_NULL, m_queue->m_head)) break;
}


static void QueueEnqueue(void* m_data, Queue* m_queue)
{
	QueueLazyInitialization(m_queue);
	if (!m_queue->m_head || !m_queue->m_tail) return;
	QueueNode* m_node = AllocateQueueNode();
	if (!m_node) return;
	m_node->m_data = m_data;
	m_node->m_next = AAE_NULL;
	QueueNode* t_node, *t_next;
	HPRecord* c_hp = acquire_hp(&m_shared_hp_list);
	if (!c_hp) return;
	while (1)
	{
		t_node = m_queue->m_tail;
		c_hp->m_hazard_ptr = t_node;
		MEMORY_FENCE;
		if (m_queue->m_tail != t_node) continue;
		t_next = t_node->m_next;
		if (m_queue->m_tail != t_node) continue;
		if (t_next) { CompareAndSwap((void* volatile*)&m_queue->m_tail, t_node, t_next); continue; }
		if (CompareAndSwap((void* volatile*)&m_queue->m_tail->m_next, AAE_NULL, m_node)) break;
	}
	CompareAndSwap((void* volatile*)&m_queue->m_tail, t_node, m_node);
	release_hp(c_hp);
}


static void* QueueDequeue(Queue* m_queue)
{
	QueueLazyInitialization(m_queue);
	if (!m_queue->m_head || !m_queue->m_tail) return AAE_NULL;
	void* m_data;
	QueueNode* t_node, *h_node, *h_next;
	HPRecord* c_hp1 = acquire_hp(&m_shared_hp_list);
	if (!c_hp1) return AAE_NULL;
	HPRecord* c_hp2 = acquire_hp(&m_shared_hp_list);
	if (!c_hp2) return AAE_NULL;
	while (1)
	{
		h_node = m_queue->m_head;
		c_hp1->m_hazard_ptr = h_node;
		MEMORY_FENCE;
		if (m_queue->m_head != h_node) continue;
		t_node = m_queue->m_tail;
		h_next = h_node->m_next;
		c_hp2->m_hazard_ptr = h_next;
		MEMORY_FENCE;
		if (m_queue->m_head != h_node) continue;
		if (!h_next) return AAE_NULL;
		if (h_node == t_node) { CompareAndSwap((void* volatile*)&m_queue->m_tail, t_node, h_next); continue; }
		m_data = h_next->m_data;
		if (CompareAndSwap((void* volatile*)&m_queue->m_head, h_node, h_next)) break;
	}
	release_hp(c_hp1);
	release_hp(c_hp2);
	InsertRetiredNode(&m_retired_queue_list, h_node);
	ScanRetiredNodes(&m_retired_queue_list, &m_shared_hp_list, DeallocateQueueNode);
	return m_data;
}


static void ListPutPartial(Descriptor* c_descriptor, uintptr_t m_size_class)
{
	uint16_t c_index = (m_size_class / AAE_GRANULARITY) - 1;
	QueueEnqueue(c_descriptor, m_queues + c_index);
}


static Descriptor* ListGetPartial(uintptr_t m_size_class)
{
	uint16_t c_index = (m_size_class / AAE_GRANULARITY) - 1;
	return QueueDequeue(m_queues + c_index);
}


static void ListRemoveEmptyDescriptor(uintptr_t m_size_class)
{
	Descriptor* c_descriptor;
	while (c_descriptor = ListGetPartial(m_size_class))
		if (c_descriptor->m_anchor.m_state != EMPTY) { ListPutPartial(c_descriptor, m_size_class); break; }
}


static void RemoveEmptyDescriptor(ProcessorHeap* c_heap, Descriptor* c_descriptor)
{
	if (CompareAndSwap((void* volatile*)&c_heap->m_partial, c_descriptor, AAE_NULL))
	{
		InsertRetiredNode(&m_retired_descriptor_list, c_descriptor);
		ScanRetiredNodes(&m_retired_descriptor_list, &m_shared_hp_list, DeallocateDescriptor);
		return;
	}
	ListRemoveEmptyDescriptor(c_heap->m_size_class);
}


static void HeapPutPartial(Descriptor* c_descriptor)
{
	Descriptor* previous;
	do
	{
		previous = c_descriptor->m_heap->m_partial;
	}while(!CompareAndSwap((void* volatile*)&c_descriptor->m_heap->m_partial, previous, c_descriptor));
	if (previous) ListPutPartial(previous, previous->m_heap->m_size_class);
}


static Descriptor* HeapGetPartial(ProcessorHeap* c_heap)
{
	Descriptor* c_descriptor;
	do
	{
		c_descriptor = c_heap->m_partial;
		if (!c_descriptor) return ListGetPartial(c_heap->m_size_class);
	}while(!CompareAndSwap((void* volatile*)&c_heap->m_partial, c_descriptor, AAE_NULL));
	return c_descriptor;
}


static void UpdateActive(ProcessorHeap* c_heap, Descriptor* c_descriptor, uint32_t more_credits)
{
	uintptr_t new_active = (uintptr_t)c_descriptor;
	set_active_credits(&new_active, more_credits - 1);
	if (CompareAndSwap((uintptr_t volatile*)&c_heap->m_active, AAE_NULL, new_active)) return;

	Anchor new_anchor, old_anchor;
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		new_anchor.m_count += more_credits;
		new_anchor.m_state = PARTIAL;
		STORE_FENCE;
	}while(!CompareAndSwap((uint64_t volatile*)&c_descriptor->m_anchor, *(uint64_t*)&old_anchor, *(uint64_t*)&new_anchor));
	HeapPutPartial(c_descriptor);
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
		if (!old_active) return AAE_NULL;
		if (get_active_credits(old_active) == 0)
			new_active = 0; /** only one block left in active superblock **/
		else
			--new_active; /** mark allocation of a block from active superblock **/
	}while(!CompareAndSwap((uintptr_t volatile*)&c_heap->m_active, old_active, new_active));
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
				more_credits = aae_min(old_anchor.m_count, MAXCREDITS);
				new_anchor.m_count -= more_credits;
			}
		}
		STORE_FENCE;
	}while(!CompareAndSwap((uint64_t volatile*)&c_descriptor->m_anchor, *(uint64_t*)&old_anchor, *(uint64_t*)&new_anchor));
	if (get_active_credits(old_active) == 0 && old_anchor.m_count > 0)
		UpdateActive(c_heap, c_descriptor, more_credits);
	*((Descriptor**)address) = c_descriptor;
	return (uintptr_t*)(address + sizeof(Descriptor*));

}


static void* AllocateFromPartialSuperBlock(ProcessorHeap* c_heap)
{
	Descriptor* c_descriptor;
	Anchor new_anchor, old_anchor;
	uint32_t more_credits;
	void* address;
	retry:
	c_descriptor = HeapGetPartial(c_heap);
	if (!c_descriptor) return AAE_NULL;
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		if (old_anchor.m_state == EMPTY)
		{
			InsertRetiredNode(&m_retired_descriptor_list, c_descriptor);
			ScanRetiredNodes(&m_retired_descriptor_list, &m_shared_hp_list, DeallocateDescriptor);
			goto retry;
		}
		more_credits = aae_min(old_anchor.m_count - 1, MAXCREDITS);
		new_anchor.m_count -= (more_credits + 1);
		new_anchor.m_state = (more_credits > 0) ? ACTIVE : FULL;
	}while(!CompareAndSwap((uint64_t volatile*)&c_descriptor->m_anchor, *(uint64_t*)&old_anchor, *(uint64_t*)&new_anchor));
	do
	{
		new_anchor = old_anchor = c_descriptor->m_anchor;
		address = c_descriptor->m_super_block + old_anchor.m_available * c_descriptor->m_block_size;
		uint32_t next = *((uintptr_t*)address);
		new_anchor.m_available = next;
		++new_anchor.m_tag;
	}while(!CompareAndSwap((uint64_t volatile*)&c_descriptor->m_anchor, *(uint64_t*)&old_anchor, *(uint64_t*)&new_anchor));
	if (more_credits > 0) UpdateActive(c_heap, c_descriptor, more_credits);
	*((Descriptor**)address) = c_descriptor;
	return (uintptr_t*)(address + sizeof(Descriptor*));

}


static void* AllocateFromNewSuperblock(ProcessorHeap* c_heap)
{
	Descriptor* c_descriptor = AllocateDescriptor();
	if (!c_descriptor) return AAE_NULL;
	c_descriptor->m_super_block = os_alloc(AAE_SUPERBLOCK_SIZE);
	if (!c_descriptor->m_super_block) return AAE_NULL;
	c_descriptor->m_heap = c_heap;
	c_descriptor->m_block_size = c_heap->m_size_class;
	c_descriptor->m_number_of_blocks = AAE_SUPERBLOCK_SIZE / c_descriptor->m_block_size;

	organize_list(c_descriptor->m_super_block, c_descriptor->m_number_of_blocks, c_descriptor->m_block_size);
	uintptr_t new_active = (uintptr_t)c_descriptor;
	set_active_credits(&new_active, (aae_min(c_descriptor->m_number_of_blocks - 1, MAXCREDITS) - 1)); /** n credits means n+1 available blocks **/
	c_descriptor->m_anchor.m_available = 1;
	c_descriptor->m_anchor.m_count = c_descriptor->m_number_of_blocks - get_active_credits(new_active) - 2;
	c_descriptor->m_anchor.m_state = ACTIVE; /** install it as the active superblock **/

	STORE_FENCE;
	if (CompareAndSwap((uintptr_t volatile*)&c_heap->m_active, AAE_NULL, new_active))
	{
		*((Descriptor**)c_descriptor->m_super_block) = c_descriptor;
		return (uintptr_t*)(c_descriptor->m_super_block + sizeof(Descriptor*));
	}
	else
	{
		os_dealloc(c_descriptor->m_super_block, AAE_SUPERBLOCK_SIZE);
		InsertRetiredNode(&m_retired_descriptor_list, c_descriptor);
		ScanRetiredNodes(&m_retired_descriptor_list, &m_shared_hp_list, DeallocateDescriptor);
		return AAE_NULL;
	}
}


static ProcessorHeap* FindHeap(size_t size) /** return the heap associated with the current cpu **/
{
	/** We need to fit both the object and the descriptor in a single block **/
	size += sizeof(Descriptor*);
	if (size >= (AAE_NUMBER_OF_SIZE_CLASSES * AAE_GRANULARITY)) return AAE_NULL; /** large block **/

	--size; /** an allocation of n * AAE_GRANULARITY should fit in the (n - 1)th size class **/
	int32_t c_cpu = get_current_cpu(); /** this might change before update but it's rather infrequent **/
	if (c_cpu == -1) return AAE_NULL; /** fallback to large block allocation **/
	c_cpu = aae_min(c_cpu, AAE_NUMBER_OF_CPUS - 1);
	return &m_heaps[c_cpu][size / AAE_GRANULARITY];
}


void* aae_malloc(size_t size)
{
	if (!size) return AAE_NULL;
	ProcessorHeap *c_heap = FindHeap(size);
	void* address;
	if (!c_heap)
	{
		size_t header_size = (sizeof(Descriptor*)) << 1;
		address = os_alloc(size + header_size);
		if (address) return AAE_NULL;
		*((uintptr_t*)address) = size; /** encode the size of the large block **/
		*((uintptr_t*)(address + (header_size >> 1))) |= 0x1; /** encode the fact that this is a large block **/
		return (uintptr_t*)(address + header_size);
	}
	while (1)
	{
		address = AllocateFromActiveSuperblock(c_heap);
		if (address) return address;
		address = AllocateFromPartialSuperBlock(c_heap);
		if (address) return address;
		address = AllocateFromNewSuperblock(c_heap);
		if (address) return address;
		if (m_allocation_error_flag == -1) {m_allocation_error_flag = 0; return AAE_NULL; }
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
			LOAD_FENCE;
			new_anchor.m_state = EMPTY;
		}
		else
		{
			new_anchor.m_count++;
		}
		STORE_FENCE;
	}while(!CompareAndSwap((uint64_t volatile*)&c_descriptor->m_anchor, *(uint64_t*)&old_anchor, *(uint64_t*)&new_anchor));
	if (new_anchor.m_state == EMPTY)
	{
		os_dealloc(c_descriptor->m_super_block, AAE_SUPERBLOCK_SIZE);
		RemoveEmptyDescriptor(c_heap, c_descriptor);
	}
	else if(old_anchor.m_state == FULL)
	{
		HeapPutPartial(c_descriptor);
	}
}


void* aae_realloc(void* start, size_t size)
{
	if (!size) { aae_free(start); return AAE_NULL; }
	if (!start) return aae_malloc(size);
	size_t old_size;
	Descriptor* c_descriptor = *((Descriptor**)(uintptr_t*)(start - sizeof(Descriptor*)));
	if (large_block(c_descriptor))
		old_size = *((uintptr_t*)(start - 2 * sizeof(Descriptor*)));
	else
		old_size = c_descriptor->m_block_size;
	if (size <= old_size) return start;
	void* new_start = aae_malloc(size);
	memcpy(new_start, start, old_size);
	aae_free(start);
	return new_start;
}


void* aae_calloc(size_t n, size_t size)
{
	size_t total_size = n * size; /** might overflow - too bad **/
	void* ptr = aae_malloc(total_size);
	if (!ptr) return AAE_NULL;
	return memset(ptr, 0, total_size);
}
