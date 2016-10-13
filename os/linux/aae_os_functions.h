#include "aae_defines.h"
#include "aae_types.h"


/**
*Wrapper for linux's sys_write
*/
#ifndef AAE_WRITE_FUNC
#define AAE_wRITE_FUNC
AAE_EXTERN_TOOL void aae_write(i32 fd, const char * buffer, aae_size_t length);
#endif
/**
*Wrapper for linux's sys_exit
*/
#ifndef AAE_EXIT_FUNC
#define AAE_EXIT_FUNC
AAE_EXTERN_TOOL void aae_exit(i32 exit_code);
#endif

