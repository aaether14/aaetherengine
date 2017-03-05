#include "misc/aae_memory.h"
#include "string/aae_string.h"
#include "misc/aae_filestream.h"
#include <stdio.h>


i32 aae_main()
{
        try
        {
                aae::filestream<aae::filesource> cout(aae::filesource(1, aae::filesource::write_only_mode));
                cout<<'l'<<" 00llasdas"<<3.14<<"---"<<i16(-25);
        }
        catch(const_byte_ptr exception)
        {
                fprintf(stderr, "%s\n", exception);
        }
        return 0;
}