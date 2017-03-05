#include "misc/aae_memory.h"
#include "string/aae_string.h"
#include "misc/aae_filestream.h"
#include <stdio.h>


i32 aae_main()
{
        try
        {
                aae::filesource fsource("text", O_WRONLY | O_TRUNC);
                aae::filestream<aae::filesource> cout(fsource);
                cout<<'l'<<" "<<3.14<<"---"<<i16(-25);
        }
        catch(const_byte_ptr exception)
        {
                fprintf(stderr, "%s\n", exception);
        }
        return 0;
}