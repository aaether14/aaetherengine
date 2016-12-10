#include "aae_string.h"



/**
*strlen implementation
*/
aae_size_t aae_strlen(const char *s)
{

	const char * sc;
	for (sc = s; *sc; ++sc);
	return sc - s;

}


/**
*strcmp implementation
*/
int aae_strcmp(const char * a, const char * b)
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
