#ifndef AAE_STRING_H
#define AAE_STRING_H


#include "misc/aae_types.h"
#include <stdarg.h>
/** strcpy implementation **/
byte_ptr aae_strcpy(byte_ptr dst, const_byte_ptr src);
/** strcat implementation **/
byte_ptr aae_strcat(byte_ptr dst, const_byte_ptr src);
/** strlen implementation **/
aae_size_t aae_strlen(const_byte_ptr s);
/** strcmp implementation -> will return the result of the lexicographical comparision -> 0 means strings are equal **/
i32 aae_strcmp(const_byte_ptr a, const_byte_ptr b);
/** atol implementation **/
i64 aae_atol(const_byte_ptr s);
/** ultoa implementation **/
byte_ptr aae_ultoa(u64 v, u64 b = 10);
/** ltoa implementation **/
byte_ptr aae_ltoa(i64 v);
/** dtoa implementation **/
byte_ptr aae_dtoa(r64 v, u64 prc); /** prc is precision of output double **/
/** vsprintf implementation **/
void aae_vsprintf(byte_ptr buf, const_byte_ptr fmt, va_list args);
/** sprintf implementation **/
void aae_sprintf(byte_ptr buf, const_byte_ptr fmt, ...);
#endif
