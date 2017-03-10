#ifndef AAE_TYPE_TRAITS_H
#define AAE_TYPE_TRAITS_H
#include "aae_types.h"


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

	template<typename T>
	struct is_real { static const bool value = false; };
	template<>
	struct is_real<r32> { static const bool value = true; };
	template<>
	struct is_real<r64> { static const bool value = true; };

	template<typename T> struct remove_reference      { using type = T; };
	template<typename T> struct remove_reference<T&>  { using type = T; };
	template<typename T> struct remove_reference<T&&> { using type = T; };
	template<typename T>
	constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
    	{
    		return static_cast<typename remove_reference<T>::type&&>(t); 
    	}
}


#endif