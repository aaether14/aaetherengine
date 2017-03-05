#ifndef AAE_TYPES_H
#define AAE_TYPES_H
#include <stdint.h>
#ifdef AAE_LINUX_PLATFORM
#include <sys/types.h>
#ifdef aae_thread_local
#undef aae_thread_local
#endif
#define aae_thread_local __thread
#endif


typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;


typedef size_t aae_size_t;
typedef ssize_t aae_ssize_t;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


typedef float r32;
typedef double r64;
typedef decltype(nullptr) nullptr_t;


typedef char byte;
typedef char* byte_ptr;
typedef const char* const_byte_ptr;



namespace aae
{

	template<bool B, typename T = void>
	struct enable_if {}; 
	template<typename T>
	struct enable_if<true, T> { using type = T; };

	template<typename T>
	struct is_signed { static const bool value = false; };
	template<>
	struct is_signed<i16> { static const bool value = true; };
	template<>
	struct is_signed<i32> { static const bool value = true; };
	template<>
	struct is_signed<i64> { static const bool value = true; };

	template<typename T>
	struct is_unsigned { static const bool value = false; };
	template<>
	struct is_unsigned<u16> { static const bool value = true; };
	template<>
	struct is_unsigned<u32> { static const bool value = true; };
	template<>
	struct is_unsigned<u64> { static const bool value = true; };
}
#endif