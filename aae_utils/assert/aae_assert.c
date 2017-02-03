#include "aae_types.h"
#include "aae_defines.h"


#ifdef AAE_LINUX_PLATFORM
enum __aae__std__files
{
	stdin = 0,
	stdout,
	stderr
};
#endif


AAE_EXTERN_TOOL void exit(i32 exit_code);
AAE_EXTERN_TOOL void write(i32 fd, const_byte_ptr buffer, aae_size_t length);
aae_size_t aae_strlen(const char * s);


void __aae_assert(const char * msg)
{
	

	write(stderr, "\nAssertion failed! - ", 20);
	write(stderr, msg, aae_strlen(msg));
	exit(1);

}