/**
*aae_defines.h - 5.08.2016 - Dinescu Stefan-Cristian aka. Aaether
*/


#ifndef AAE_DEFINES_H
#define AAE_DEFINES_H


#ifndef AAE_EXTERN_TOOL
#define AAE_EXTERN_TOOL extern "C"
#endif


#ifndef AAE_NULL
#define AAE_NULL nullptr
#endif


#ifndef stdin
#define stdin 0
#endif


#ifndef stdout
#define stdout 1
#endif


#ifndef stderr
#define stderr 2
#endif


#define TOKENPASTE(x, y) x##y
#define INDIRECT_TOKENPASTE(x, y) TOKENPASTE(x, y)


#define aae_fiz(z) for(int i = 0; i < z; i++)
#define aae_fjz(z) for(int j = 0; j < z; j++)
#define aae_fkz(z) for(int k = 0; k < z; k++)


#endif
