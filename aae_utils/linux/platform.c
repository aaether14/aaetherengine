#include "aae_memory_allocator.h"
#include "aae_allocators.h"
#include "string/aae_string.h"
#include "ctype/aae_ctype.h"
#include "../os/linux/aae_os_functions.h" 


extern aae_allocator aae_mallocator;



i32 aae_main()
{
	int* xxx = AAE_NEW_ARRAY(int, 9, aae_mallocator);
        int* yyy = AAE_NEW_ARRAY(int, 9, aae_mallocator);
        byte* buffer = AAE_NEW_ARRAY(byte, 1000, aae_mallocator);
        aae_sprintf(buffer, "%x, %x\n%2f, %4F\n", xxx, yyy, 3.5, 55.2332f);
        aae_write(stderr, buffer, aae_strlen(buffer));
        AAE_DELETE_ARRAY(xxx, aae_mallocator);
        AAE_DELETE_ARRAY(yyy, aae_mallocator);
        AAE_DELETE_ARRAY(buffer, aae_mallocator);
        return 0;
}