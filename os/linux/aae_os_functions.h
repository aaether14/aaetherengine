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
AAE_EXTERN_TOOL void aae_write(i32 fd, const char * buffer, aae_size_t length);
#endif
/** *Wrapper for linux's sys_exit **/
#ifndef AAE_EXIT_FUNC
#define AAE_EXIT_FUNC
AAE_EXTERN_TOOL void aae_exit(i32 exit_code);
#endif
