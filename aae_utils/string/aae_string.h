#ifndef AAE_STRING_H
#define AAE_STRING_H


#include "aae_types.h"
/**
*strlen implementation
**/
aae_size_t aae_strlen(const_byte_ptr s);
/**
*strcmp implementation
*will return < 0 if the strings are equal, < 0 if a is "smaller" than b and > 0 otherwise
**/
i32 aae_strcmp(const_byte_ptr a, const_byte_ptr b);
#endif
