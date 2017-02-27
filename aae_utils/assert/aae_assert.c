#include "aae_types.h"
#include "aae_defines.h"


#ifdef AAE_LINUX_PLATFORM
AAE_EXTERN_TOOL void exit(i32 exit_code);
AAE_EXTERN_TOOL void write(i32 fd, const_byte_ptr buffer, aae_size_t length);
#endif


aae_size_t aae_strlen(const_byte_ptr s);
void __aae_assert(const_byte_ptr msg)
{
	

	write(2, "\nAssertion failed! - ", 20);
	write(2, msg, aae_strlen(msg));
	exit(1);

}