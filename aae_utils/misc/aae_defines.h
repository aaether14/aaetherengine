#ifndef AAE_DEFINES_H
#define AAE_DEFINES_H
#ifdef AAE_EXTERN_TOOL
#undef AAE_EXTERN_TOOL
#endif
#define AAE_EXTERN_TOOL extern "C"
#ifdef AAE_NULL
#undef AAE_NULL
#endif
#define AAE_NULL nullptr
#ifdef aae_fiz 
#undef aae_fiz
#endif
#ifdef aae_fjz 
#undef aae_fjz
#endif
#ifdef aae_fkz 
#undef aae_fkz
#endif
#define aae_fiz(z) for(int i = 0; i < z; i++)
#define aae_fjz(z) for(int j = 0; j < z; j++)
#define aae_fkz(z) for(int k = 0; k < z; k++)
#endif
