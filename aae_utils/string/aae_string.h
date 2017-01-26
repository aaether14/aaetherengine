#ifndef AAE_STRING_H
#define AAE_STRING_H


#include "aae_types.h"
/** strlen implementation **/
aae_size_t aae_strlen(const_byte_ptr s);
/** strcmp implementation -> will return the result of the lexicographical comparision -> 0 means strings are equal **/
i32 aae_strcmp(const_byte_ptr a, const_byte_ptr b);
/** atol implementation **/
i64 aae_atoi(const_byte_ptr s);
/** ultoa implementation **/
byte_ptr aae_ultoa(u64 v, aae_size_t o = 0ull); /** warning! if o is non-zero the output string will not be zero-terminated **/
/** ltoa implementation **/
byte_ptr aae_ltoa(i64 v);
/** dtoa implementation **/
byte_ptr aae_dtoa(r64 v, u64 prc); /** prc is precision of output double **/
#endif
