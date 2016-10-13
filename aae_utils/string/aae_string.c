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
