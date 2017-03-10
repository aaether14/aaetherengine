#include "misc/aae_memory.h"
#include "string/aae_string.h"
#include "misc/aae_filestream.h"
#include <stdio.h>


i32 aae_main()
{
        try
        {
                aae::filesource test1(1, aae::filesource::write_only_mode);
                aae::filesource test2 = aae::move(test1);
                aae::filestream<aae::filesource> cout(test2);
                cout << "Hello World!" << " " << 3.15 << " " << -255 << aae::flush;
        }
        catch(const_byte_ptr exception)
        {
                fprintf(stderr, "%s\n", exception);
        }
        return 0;
}