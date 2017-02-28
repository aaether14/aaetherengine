#ifndef AAE_OS_FUNCTIONS_H
#define AAE_OS_FUNCTIONS_H
#include "misc/aae_defines.h"
#include "misc/aae_types.h"
#ifdef AAE_LINUX_PLATFORM
enum __aae__std__files
{
	stdin = 0,
	stdout,
	stderr
};
#endif
/** *Wrapper for linux's sys_write **/
#ifdef aae_write
#undef aae_write
#endif
#define aae_write write
AAE_EXTERN_TOOL aae_size_t aae_write(i32, const_byte_ptr, aae_size_t);
/** *Wrapper for libc's sys_exit **/
#ifdef aae_exit
#undef aae_exit
#endif
#define aae_exit exit
AAE_EXTERN_TOOL void aae_exit(i32);
#endif
