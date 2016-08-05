#include <stdio.h>
#include "aae_utils/aae_linked_list_type_generation.h"



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{

	
	aae_LinkedList(float) * pList = aae_LinkedListNewList(float);
	aae_LinkedListPushBack(float, pList, 2.25);
	aae_LinkedListPushBack(float, pList, 3.14);
	aae_LinkedListPushBack(float, pList, 1241.32);
	aae_LinkedListPushBack(float, pList, 123.321);	


	aae_LinkedListForEach(pList)
	{
		printf("%2.2f\n", pList->m_it->m_data);
	}


	aae_LinkedListFreeNodes(float, pList);	
	free(pList);
	pList = NULL;


 
	return 0;


}
