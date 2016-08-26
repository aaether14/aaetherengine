/**
*general purpose allocator by Stefan Cristian Dinescu aka Aaether
*can allocate a max 4gb of memory
*/



/*
*Include the sbrk function 
*/ 
#include <unistd.h> 
#include <stdint.h>




static uint32_t aae_alloc_has_initialized = 0;
static void *aae_alloc_managed_memory_start;
static void *aae_alloc_last_valid_address;





struct aae_alloc_mem_control_block 
{ 
	uint32_t is_available; 
	uint32_t size;
};





void aae_free(void *firstbyte) 
{

	/*
	*if provided with a null pointer return
	*/
	if (!firstbyte)
		return;


	struct aae_alloc_mem_control_block *mcb;  
	/*
	*backup from the given pointer to find the 
	* mem_control_block 
	*/ 
	mcb = firstbyte - sizeof(struct aae_alloc_mem_control_block); 


	/*
	*mark the block as being available
	*/ 
	mcb->is_available = 1;     
	return;


}  




void *aae_malloc(uint32_t numbytes) 
{ 


	/* 
	*initialize if we haven't done so
	*/
	if(!aae_alloc_has_initialized)
	{

		/*
		*get the system break from the OS
		*/
		aae_alloc_last_valid_address = sbrk(0);     


		/* 
		*we don't have any memory to manage yet, so 
		*just set the beginning to be last_valid_address 
		*/  
		aae_alloc_managed_memory_start = aae_alloc_last_valid_address;     


		/* 
		*done initializing
		*/
		aae_alloc_has_initialized = 1;

	}
	


	/*
	*current memory location we're working with
	*/
	union 
	{
		void * as_void;
		struct aae_alloc_mem_control_block *as_mcb;  
	} location;



	/*
	*this is the memory location we return, will hold 0 untill 
	*we find a suitable location
	*/
	void *memory_location = ((void*)0);  
	 


	/*
	*we also need to allocate memory for the control block
	*/
	numbytes = numbytes + sizeof(struct aae_alloc_mem_control_block);  

	/*
	*we begin searching at the start of the managed memory
	*/ 
	location.as_void = aae_alloc_managed_memory_start;





	/*
	*keep going until we have searched all allocated space
	*/ 
	while(location.as_void != aae_alloc_last_valid_address)  
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
		*Move the program break numbytes further 
		*/
		sbrk(numbytes);


		/*
		*the new memory will be where the last valid 
		* address left off 
		*/
		memory_location = aae_alloc_last_valid_address;


		/*
		*we'll move the last valid address forward 
		*numbytes 
		*/
		aae_alloc_last_valid_address = aae_alloc_last_valid_address + numbytes;
		location.as_mcb = memory_location;
		location.as_mcb->is_available = 0;
		location.as_mcb->size = numbytes;


	}

	

	/*
	*move the pointer past the mem_control_block 
	*/
	memory_location = memory_location + sizeof(struct aae_alloc_mem_control_block);


	/*
	*Return the pointer 
	*/
	return memory_location;


}



uint32_t aae_memory_used()
{
	return (uint32_t)(aae_alloc_last_valid_address - aae_alloc_managed_memory_start);
}
