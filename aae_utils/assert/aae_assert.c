/**
*aae_assert.c
*/
#include "aae_types.h"
#include "aae_defines.h"



AAE_EXTERN_TOOL void aae_exit(i32 exit_code);
AAE_EXTERN_TOOL void aae_write(i32 fd, const char * buffer, aae_size_t length);
aae_size_t aae_strlen(const char * s);



void __aae_assert(const char * msg)
{
	

	aae_write(stderr, "\nAssertion failed! - ", 20);
	aae_write(stderr, msg, aae_strlen(msg));


	aae_exit(1);

}




