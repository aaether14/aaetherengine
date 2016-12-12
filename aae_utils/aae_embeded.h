/**
*This will only work with GCC - accesing files embeded in the executable using the symbols defined here
**/
#ifndef AAE_EMBEDED_H
#define AAE_EMBEDED_H


#ifdef __GNUC__ 
extern unsigned char _binary_blob_bin_start;
extern unsigned char _binary_blob_bin_end;
extern unsigned char _binary_blob_bin_size;
#endif 


#endif
