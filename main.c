#include <stdio.h>
#include "aae_utils/aae_linked_list.h"


AAE_DEFINE_LINKED_LIST(float)



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{


	aae_LinkedList(float) * pList = aae_NewLinkedList(float);
	aae_LinkedListPushBack(float, pList, 5.32); 
	aae_LinkedListPushBack(float, pList, 3.14); 
        aae_LinkedListPushFront(float,pList, 24521.15);
	aae_LinkedListPushBack(float, pList, 1.72);
	aae_LinkedListPushBack(float, pList, 213213.23);
	aae_LinkedListPushFront(float, pList, 3.152);


	for (pList->m_it = pList->m_first; pList->m_it!= NULL; pList->m_it = pList->m_it->m_next)
	printf("%2.3f\n", pList->m_it->m_data);
	printf("There are %lu members in the list.\n", pList->m_size);


	aae_LinkedListFreeNodes(float, pList);
	free(pList);
	pList = NULL;

 
	return 0;


}
