#ifndef AAE_ASSERT_H
#define AAE_ASSERT_H


void __aae_assert(const char * msg);


#endif	/** AAE_ASSERT_H **/



#ifdef AAE_ASSERT
#undef AAE_ASSERT
#endif


#ifdef AAE_RELEASE_BUILD
#define AAE_ASSERT(test) ((void)0)
#else
#define __STR(x) __VAL(x)
#define __VAL(x) #x
#define AAE_ASSERT(test) ((test) ? (void)0 : __aae_assert(__FILE__ " : " __STR(__LINE__) " - " #test "\n"))
#endif
