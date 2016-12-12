#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "assert/aae_assert.h"
#include "error/aae_error.h"
#include "os/linux/aae_os_functions.h" 
#include <pthread.h>


extern aae_allocator aae_mallocator;



#define AAE_LINKED_LIST_TYPE r32
#define AAE_LINKED_LIST_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_linked_list.def"



#define AAE_RB_TREE_KEY_TYPE r32
#define AAE_RB_TREE_VALUE_TYPE const_byte_ptr
#define AAE_RB_TREE_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_rb_tree.def"



void* function(void* key)
{
	
	aae_RBTree(r32, const_byte_ptr) *test = AAE_NEW(aae_RBTree(r32, const_byte_ptr), aae_mallocator);
	test->insert(5.2, "This is a message!\n");
	test->insert(7.45, "Thread 2:");
	test->insert(9.13, "lol9");
	test->insert(3.14, "Fucking panzers!");
	test->insert(2.113, "Lolipop!");
	test->insert(1.71, "Hello World!\n");
	AAE_DELETE(test, aae_mallocator);
	return AAE_NULL;
}



AAE_EXTERN_TOOL i32 aae_main()
{


	pthread_t thread_id;
	pthread_create(&thread_id, AAE_NULL, &function, AAE_NULL);
	pthread_join(thread_id, AAE_NULL);
	return 0;

}
