#ifndef AAE_ERROR_H
#define AAE_ERROR_H


#include "aae_types.h"


i32 aae_get_error();
void aae_set_error(i32); 
#define AAE_COULD_NOT_GET_THREAD_SPECIFIC_MEMORY 255;


#endif /** AAE_ERROR_H **/