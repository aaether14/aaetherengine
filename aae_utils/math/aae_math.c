#include "aae_math.h"


u64 aae_pow10(u64 power)
{
	static const aae_thread_local r64 m_pow10_arr[17] = {
		1e0, 1e1, 1e2, 1e3, 1e4, 1e5,
		1e6, 1e7, 1e8, 1e9, 1e10, 1e11,
		1e12, 1e13, 1e14, 1e15, 1e16 
	};
	if (power > 16) return 0;
	return ((u64)m_pow10_arr[power]); 
}