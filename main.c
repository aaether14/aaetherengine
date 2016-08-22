#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"
#include "aae_utils/aae_memory_allocator.h"


//extern variable from aae_memory_allocator.h
aae_allocator mallocator;



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{



	aae_LinkedList(float) * pList = AAE_NEW(aae_LinkedList(float), mallocator);
	pList->push_back(1.5);
	pList->push_front(2.3);
	pList->push_back(1.23);
	pList->push_front(3.14);
	pList->push_back(231.132);

	
	aae_LinkedListForEach(pList)
	{
		printf("%2.2f\n", pList->m_it->m_data);
	}


	aae_LinkedListFreeNodes(float, pList);	
	free(pList);
	pList = NULL;
 

 
	return 0;


}
