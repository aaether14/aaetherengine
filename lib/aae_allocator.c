/**
*linux general purpose allocator by Stefan Cristian Dinescu aka Aaether
*/



/*
*Include the sbrk function 
*/ 
#include <unistd.h> 



#ifdef __GNUC__ 
typedef __SIZE_TYPE__ aae_size_t;
#endif



#ifndef AAE_INTERN_TOOL
#define AAE_INTERN_TOOL static
#endif



#ifndef AAE_NULL
#define AAE_NULL ((void*)0);  
#endif



#ifndef AAE_SBRK_ERROR
#define AAE_SBRK_ERROR ((void*)-1)
#endif



AAE_INTERN_TOOL int has_initialized = 0;
AAE_INTERN_TOOL void *managed_memory_start;
AAE_INTERN_TOOL void *last_valid_address;



struct mem_control_block 
{ 
	aae_size_t size;
	int is_available; 
};



union __memory_location 
{
	void * as_void;
	struct mem_control_block *as_mcb;  
};




void aae_free(void *firstbyte) 
{

	/*
	*if provided with a null pointer return
	*/
	if (!firstbyte)
		return;


	/*
	*backup from the given pointer to find the 
	* mem_control_block 
	*/ 
	struct mem_control_block *mcb = firstbyte - sizeof(struct mem_control_block); 


	/*
	*mark the block as being available
	*/ 
	mcb->is_available = 1;     
	return;


}  




void *aae_malloc(aae_size_t numbytes) 
{ 


	/* 
	*initialize if we haven't done so
	*/
	if(!has_initialized)
	{

		/*
		*get the system break from the OS and if any error ocures return NULL
		*/
		last_valid_address = sbrk(0);     
		if (last_valid_address == AAE_SBRK_ERROR)
		{	
			last_valid_address = AAE_NULL;
			return AAE_NULL;

		}
		/* 
		*we don't have any memory to manage yet, so 
		*just set the beginning to be last_valid_address 
		*/  
		managed_memory_start = last_valid_address;     


		/* 
		*done initializing
		*/
		has_initialized = 1;

	}


	
	/*
	*this is the memory location we return, will hold 0 untill 
	*we find a suitable location
	*/
	void *memory_location = AAE_NULL;
	 

	/*
	*we also need to allocate memory for the control block
	*/
	numbytes = numbytes + sizeof(struct mem_control_block);  



	/*
	*current memory location we're working with
	*/
	union __memory_location location;


	/*
	*we begin searching at the start of the managed memory
	*/ 
	location.as_void = managed_memory_start;



	/*
	*keep going until we have searched all allocated space
	*/ 
	while(location.as_void != last_valid_address)  
	{


		if(location.as_mcb->is_available)
		{

			if(location.as_mcb->size >= numbytes)
			{

				location.as_mcb->is_available = 0;
				memory_location = location.as_void;
				break;


			}
		}


		/* 
		*if we made it here, it's because the Current memory 
		*block not suitable, move to the next one 
		*/
		location.as_void = location.as_void + location.as_mcb->size;

	}




	/* 
	*if we still don't have a valid location, we'll 
	* have to ask the operating system for more memory 
	*/
	if(!memory_location)
	{


		/* 
		*Move the program break numbytes further, and if any error ocures return NULL 
		*/
		if(sbrk(numbytes) == AAE_SBRK_ERROR)
			return AAE_NULL;


		/*
		*the new memory will be where the last valid 
		* address left off 
		*/
		memory_location = last_valid_address;


		/*
		*we'll move the last valid address forward 
		*numbytes 
		*/
		last_valid_address = last_valid_address + numbytes;
		location.as_mcb = memory_location;
		location.as_mcb->is_available = 0;
		location.as_mcb->size = numbytes;


	}

	

	/*
	*move the pointer past the mem_control_block 
	*/
	memory_location = memory_location + sizeof(struct mem_control_block);


	/*
	*Return the pointer 
	*/
	return memory_location;


}



aae_size_t aae_allocated_memory()
{
	return (aae_size_t)(last_valid_address - managed_memory_start);
}
