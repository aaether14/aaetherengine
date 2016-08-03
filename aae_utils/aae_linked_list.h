/**
---aae_linked_list.h - Dinescu Stefan-Cristian aka Aaether - 31.07.2016
----------------------------------------------------------------------
Code generation for linked list holding TYPE data
-td stands for typedef
-don't ever use variables, functions etc. starting with "__". Only the framework has this right.
Features:
-push a new node to the front of the list
-push a new node to the back of the list
Safe types:
-aae_LinkedList(TYPE)
-aae_LinkedListElement(TYPE)
*/





#ifndef AAE_LINKED_LIST_H
#define AAE_LINKED_LIST_H
#include "aae_types.h"
#include <malloc.h>


/**
*Posible return values for the linked list
*/
typedef enum
	{	AAE_LINKED_LIST_ALLOCATION_ERROR = -2, //malloc error
		AAE_LINKED_LIST_INVALID_LIST = -1, //invalid list pointer
		AAE_LINKED_LIST_FALSE = 0,
		AAE_LINKED_LIST_TRUE = 1,
		AAE_LINKED_LIST_SUCCESS = 2
	} AAE_LINKED_LIST_RETURN_VALUES;
#endif /* AAE_LINKED_LIST_H*/






#ifndef AAE_LINKED_LIST_TYPE
#error "Must define AAE_LINKED_LIST_TYPE!"
#endif



/**
*Indirection is used in order to achieve defined token concatenation
*/
#define TOKENPASTE(x, y) x##y
#define INDIRECT_TOKENPASTE(x, y) TOKENPASTE(x, y)
#define __AAE_LINKED_LIST_PREFIX(NAME) INDIRECT_TOKENPASTE(INDIRECT_TOKENPASTE(__linked_list, NAME), AAE_LINKED_LIST_TYPE)
#define __AAE_LINKED_LIST_PREFIX_TD(NAME) INDIRECT_TOKENPASTE(INDIRECT_TOKENPASTE(__td_linked_list, NAME), AAE_LINKED_LIST_TYPE)




	typedef struct __AAE_LINKED_LIST_PREFIX(_elem)
	{
		AAE_LINKED_LIST_TYPE m_data;
		__AAE_LINKED_LIST_PREFIX(_elem) * m_next;
	} __AAE_LINKED_LIST_PREFIX_TD(_elem);



	typedef struct __AAE_LINKED_LIST_PREFIX(_)
	{
		aae_size_t m_size;
		__AAE_LINKED_LIST_PREFIX_TD(_elem) * m_first;
		__AAE_LINKED_LIST_PREFIX_TD(_elem) * m_last;
		__AAE_LINKED_LIST_PREFIX_TD(_elem) * m_it;
	} __AAE_LINKED_LIST_PREFIX_TD(_);



	AAE_LINKED_LIST_RETURN_VALUES __AAE_LINKED_LIST_PREFIX(_push_back)(__AAE_LINKED_LIST_PREFIX_TD(_) * pList, AAE_LINKED_LIST_TYPE tValue)
	{

		if (!pList) return AAE_LINKED_LIST_INVALID_LIST;


		__AAE_LINKED_LIST_PREFIX_TD(_elem) * pNewElem = (__AAE_LINKED_LIST_PREFIX_TD(_elem)*)malloc(sizeof(__AAE_LINKED_LIST_PREFIX_TD(_elem)));
		if (!pNewElem) return AAE_LINKED_LIST_SUCCESS;


		pNewElem->m_next = NULL; pNewElem->m_data = tValue;
		if (!pList->m_first) pList->m_first = pList->m_last = pNewElem;
		else { pList->m_last->m_next = pNewElem; pList->m_last = pNewElem; }


		pList->m_size++;
		return AAE_LINKED_LIST_SUCCESS;
	}



	__AAE_LINKED_LIST_PREFIX_TD(_) * __AAE_LINKED_LIST_PREFIX(_new_list)()
	{
		
		__AAE_LINKED_LIST_PREFIX_TD(_) * pNewList = (__AAE_LINKED_LIST_PREFIX_TD(_)*)malloc(sizeof(__AAE_LINKED_LIST_PREFIX_TD(_)));
		pNewList->m_first = pNewList->m_last = pNewList->m_it = NULL;									
		pNewList->m_size = 0;														
		return pNewList; 	

	}


/**
*Types
*/
#define aae_LinkedList(TYPE)					__td_linked_list_##TYPE //Defines a linked list of a certain TYPE
#define aae_LinkedListElement(TYPE)				__td_linked_list_elem_##TYPE //Defines a node holding data of TYPE
/**
List members (better don't modify this using other than defined functions):
m_first - the first node in the list
m_last - the last node in the list
m_size - the size of the list
*/



/**
*Functions
*/
#define aae_LinkedListPushBack(TYPE, list, elem)		__linked_list_push_back##TYPE(list, elem)
#define aae_LinkedListNewList(TYPE)				__linked_list_new_list##TYPE()




#ifdef AAE_LINKED_LIST_TYPE
#undef AAE_LINKED_LIST_TYPE
#endif
