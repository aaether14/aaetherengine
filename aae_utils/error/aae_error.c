#include "aae_error.h"
#ifdef AAE_LINUX_PLATFORM
static __thread i32 m_last_error_code = 0;
#endif
i32 aae_get_error()
{
	return m_last_error_code;
}
void aae_set_error(i32 error_code)
{
	m_last_error_code = error_code;
}