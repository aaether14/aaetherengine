#ifndef AAE_TYPES_H
#define AAE_TYPES_H


#include <stdint.h>



typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;



typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


/**
*Not really a guarantee
*/
typedef float r32;
typedef double r64;


typedef char byte;
typedef const char* const_byte_ptr;



#ifdef __GNUC__ 
typedef __SIZE_TYPE__ aae_size_t;
#endif




#endif
