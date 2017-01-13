#include "aae_string.h"



/** strlen implementation **/
aae_size_t aae_strlen(const_byte_ptr s)
{

	const_byte_ptr sc;
	for (sc = s; *sc; ++sc);
	return sc - s;

}


/** strcmp implementation **/
i32 aae_strcmp(const_byte_ptr a, const_byte_ptr b)
{
	while (*a)
	{
		if (*a == *b)
		{
			++a;
			++b;
		}
		else
		{
			return *a - *b;
		}
	}
	return *b ? -1 : 0;
}


/** atoi implementation **/
i32 aae_atoi(const_byte_ptr s)
{
	i32 value = 0;
	i32 sign = 1;
	const_byte_ptr p = s;
	while (s && ((*p >= '0' && *p <= '9') || (p == s && *p == '-')))
	{
		if (p == s && *p == '-')
		{
			sign = -1;
		}
		else
		{
			value *= 10;
			value += (i32)(*p - '0');
		}
		++p;
	}
	return value * sign;
}