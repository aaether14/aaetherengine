/**
*aae_defines.h - 5.08.2016 - Dinescu Stefan-Cristian aka. Aaether
*/


#ifndef AAE_DEFINES_H
#define AAE_DEFINES_H



#define TOKENPASTE(x, y) x##y
#define INDIRECT_TOKENPASTE(x, y) TOKENPASTE(x, y)



#define aae_fiz(z) for(int i = 0; i < z; i++)
#define aae_fjz(z) for(int j = 0; j < z; j++)
#define aae_fkz(z) for(int k = 0; k < z; k++)



#endif
