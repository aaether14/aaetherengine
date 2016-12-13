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



#define AAE_RB_TREE_NAME test_tree
#define AAE_RB_TREE_KEY_TYPE i32
#define AAE_RB_TREE_VALUE_TYPE const_byte_ptr
#define AAE_RB_TREE_MEMORY_ARENA aae_mallocator
#include "aae_utils/defs/aae_rb_tree.def"



void* function(void* key)
{
	
	test_tree *test = AAE_NEW(test_tree, aae_mallocator);
	test->insert(52, "This is a message! - ");
	test->insert(745, "Thread 2: - ");
	test->insert(913, "lol9 - ");
	test->insert(314, "Fucking panzers! - ");
	test->insert(2113, "Lolipop! - ");
	test->insert(171, "Hello World3! - ");
	test->insert(171, "lol55 - ");
	test->insert(823, "This is a test! - ");


	test_tree::__node* n = test->__internal_search(test->m_root, 52);
	for (;n!=test->m_nil;n=__rb_tree_next(test, n))
	{
		aae_write(stderr, n->value, aae_strlen(n->value));
		aae_write(stderr, ((n->color == 0) ? "Black\n" : "Red\n"), ((n->color == 0) ? 6 : 4));
	}


	AAE_DELETE(test, aae_mallocator);
	return AAE_NULL;
}



AAE_EXTERN_TOOL i32 aae_main()
{


	pthread_t thread_id;
	pthread_create(&thread_id, AAE_NULL, &function, NULL);
	pthread_join(thread_id, NULL);
	return 0;

}
