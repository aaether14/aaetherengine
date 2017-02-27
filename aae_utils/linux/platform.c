#include "aae_memory.h"
#include "string/aae_string.h"
#include "ctype/aae_ctype.h"
#include "../os/linux/aae_os_functions.h" 


i32 aae_main()
{
	i32* xxx = AAE_NEW_ARRAY(i32, 9, aae_mallocator);
        i32* yyy = AAE_NEW_ARRAY(i32, 9, aae_mallocator);
        i64* ttt = AAE_ALIGNED_NEW(i64, 256, aae_mallocator);
        byte* buffer = AAE_NEW_ARRAY(byte, 1000, aae_mallocator);
        aae_sprintf(buffer, "%x, %x, %x, %x\n%2f, %4F\n", xxx, yyy, ttt, ttt[-1], 3.5, 55.2332f);
        AAE_ALIGNED_DELETE(ttt, aae_mallocator);
        aae_write(stderr, buffer, aae_strlen(buffer));
        AAE_DELETE_ARRAY(xxx, aae_mallocator);
        AAE_DELETE_ARRAY(yyy, aae_mallocator);
        AAE_DELETE_ARRAY(buffer, aae_mallocator);
        return 0;
}