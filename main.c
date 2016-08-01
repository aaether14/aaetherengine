#include <stdio.h>
#include "aae_linked_list.h"
//#include "test.h"


AAE_DEFINE_LINKED_LIST(double)



int main(
    int     argc,
    char   *argv[],
    char   *envp[]
    )
{


	aae_LinkedList(double) * pList = aae_NewLinkedList(double);
	aae_LinkedListPushBack(double, pList, 5.32); 
	aae_LinkedListPushBack(double, pList, 3.14); 
	aae_LinkedListPushFront(double,pList, 2452141.323);
	aae_LinkedListPushBack(double, pList, 1.72);
	aae_LinkedListPushBack(double, pList, 213213.23);
	aae_LinkedListPushFront(double, pList, 3.152);


	for (aae_LinkedListElement(double) *it = pList->m_first; it!= NULL; it = it->m_next)
	printf("%2.3f\n", it->m_data);
	printf("There are %lu members in the list.\n", pList->m_size);


	aae_LinkedListFreeNodes(double, pList);
	free(pList);
	pList = NULL;

 
	return 0;


}
