#ifndef AAE_ASSERT_H
#define AAE_ASSERT_H
void __aae_assert(const char * msg);
#endif
#ifdef AAE_ASSERT
#undef AAE_ASSERT
#endif
#if defined(AAE_RELEASE_BUILD)
#define AAE_ASSERT(test) ((void)0)
#elif defined(AAE_DEBUG_BUILD)
#ifdef __STR
#undef __STR
#endif
#ifdef __VAL
#undef __VAL
#endif
#define __STR(x) __VAL(x)
#define __VAL(x) #x
#define AAE_ASSERT(test) ((test) ? ((void)0) : __aae_assert(__FILE__ " : " __STR(__LINE__) " - " #test "\n"))
#endif
