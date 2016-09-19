#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"


//extern variable from aae_memory_allocator.h
aae_allocator aae_mallocator;



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{


	aae_LinkedList(float) * test_list = AAE_NEW(aae_LinkedList(float), aae_mallocator);


	test_list->push_back(1.2);
	test_list->push_back(1.3);
	test_list->push_back(1.4);
	aae_LinkedListForEach(test_list){
		printf("%2.2f\n", test_list->m_it->m_data);
	}


	AAE_DELETE(test_list, aae_mallocator);
	printf("%u\n", aae_allocated_memory());



	return 0;


}
