#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "assert/aae_assert.h"
#include "error/aae_error.h"
#include "os/linux/aae_os_functions.h" 
#include <pthread.h>


extern aae_allocator aae_mallocator;


#define AAE_LINKED_LIST_NAME test_list
#define AAE_LINKED_LIST_DATA_TYPE const_byte_ptr
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
	test->insert(52, "52 - ");
	test->insert(745, "745 - ");
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->remove(913);
	test->remove(55);
	test->remove(1000);
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);

	test_tree::__node* n = test->__internal_search(test->m_root, 52);
	for (;n!=test->m_nil;n=__rb_tree_next(test, n))
	{
		aae_write(stderr, n->value, aae_strlen(n->value));
		aae_write(stderr, ((n->color == 0) ? "Black " : "Red "), ((n->color == 0) ? 6 : 4));
		if (n->left != test->m_nil)
			aae_write(stderr, n->left->value, aae_strlen(n->left->value));
		if (n->right != test->m_nil)
			aae_write(stderr, n->right->value, aae_strlen(n->right->value));
		aae_write(stderr, "\n", 1);
	}


	AAE_DELETE(test, aae_mallocator);
	return AAE_NULL;
}



AAE_EXTERN_TOOL i32 aae_main()
{
	pthread_t thread_id;
	pthread_create(&thread_id, AAE_NULL, &function, NULL);
	test_tree *test = AAE_NEW(test_tree, aae_mallocator);
	test->insert(52, "52 - ");
	test->insert(745, "745 - ");
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->remove(913);
	test->remove(55);
	test->remove(1000);
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(2342, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(82241);
	test->insert(915, "913 - ");
	test->remove(914);
	test->remove(55);
	test->remove(1000);
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823012, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "10lol00 - ");
	test->insert(172, "172 - ");
	test->remove(823);
	test->insert(913, "913 - ");
	test->insert(314, "314 - ");
	test->insert(2113, "2113 - ");
	test->insert(171, "171 - ");
	test->insert(171, "171 - ");
	test->insert(823, "823 - ");
	test->insert(2341, "2341 - ");
	test->insert(55, "55 - ");
	test->remove(314);
	test->insert(1000, "1000 - ");
	test->insert(172, "172 - ");
	test->remove(823);

	test_tree::__node* n = test->__internal_search(test->m_root, 52);
	for (;n!=test->m_nil;n=__rb_tree_next(test, n))
	{
		aae_write(stderr, n->value, aae_strlen(n->value));
		aae_write(stderr, ((n->color == 0) ? "Black " : "Red "), ((n->color == 0) ? 6 : 4));
		if (n->left != test->m_nil)
			aae_write(stderr, n->left->value, aae_strlen(n->left->value));
		if (n->right != test->m_nil)
			aae_write(stderr, n->right->value, aae_strlen(n->right->value));
		aae_write(stderr, "\n", 1);
	}


	AAE_DELETE(test, aae_mallocator);
	pthread_join(thread_id, NULL);
	return 0;
}
