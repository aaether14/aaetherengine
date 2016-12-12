#include "aae_string.h"



/**
*strlen implementation
**/
aae_size_t aae_strlen(const_byte_ptr s)
{

	const_byte_ptr sc;
	for (sc = s; *sc; ++sc);
	return sc - s;

}


/**
*strcmp implementation
**/
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


/**
*itoa implementation
**/
byte_ptr aae_itoa(i32 value, i32 base)
{
    static byte buffer[32] = {0};
    i32 i = 31;

    for(;value && i; --i, value /= base)
        buffer[i] = "0123456789abcdef"[value % base];

    return &buffer[i+1];
}