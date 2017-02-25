#include "aae_defines.h"
#include "aae_types.h"


#ifdef AAE_LINUX_PLATFORM
enum __aae__std__files
{
	stdin = 0,
	stdout,
	stderr
};
#endif

/** *Wrapper for linux's sys_write **/
#ifndef AAE_WRITE_FUNC
#define AAE_WRITE_FUNC
#define aae_write write
AAE_EXTERN_TOOL aae_size_t write(i32, const_byte_ptr, aae_size_t);
#endif
/** *Wrapper for libc's sys_exit **/
#ifndef AAE_EXIT_FUNC
#define AAE_EXIT_FUNC
#define aae_exit exit
AAE_EXTERN_TOOL void aae_exit(i32);
#endif
