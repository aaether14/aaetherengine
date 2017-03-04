#include "misc/aae_memory.h"
#include "string/aae_string.h"
#include "misc/aae_filestream.h"
#include <stdio.h>


i32 aae_main()
{
        try
        {
                aae::filestream<512> cout(1, AAE_WRITE_ONLY);
                byte b;
                cout<<'l'<<'o'<<'l'<<" "<<"is the new "<< "shit "<<25.140<<" "<<aae::float_precision(2)<<3.1423210<<" --- " << 25 << aae::flush;
        }
        catch(const_byte_ptr exception)
        {
                fprintf(stderr, "%s\n", exception);
        }
        return 0;
}