#ifndef AAE_STRING_H
#define AAE_STRING_H


#include "aae_types.h"


/**
*strlen implementation
*/
aae_size_t aae_strlen(const char * s);
/**
*strcmp implementation
*will return < 0 if the strings are equal, < 0 if a is "smaller" than b and > 0 otherwise
*/
int aae_strcmp(const char * a, const char * b);



#endif
