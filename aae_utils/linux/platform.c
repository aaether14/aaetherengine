#include "misc/aae_memory.h"
#include "string/aae_string.h"
#include "misc/aae_filestream.h"
#include <stdio.h>


i32 aae_main()
{
        try
        {
                using namespace::aae;
                filesource test1(1, filesource::write_only_mode);
                filesource test2 = aae::move(test1);
                filestream<filesource> cout(test2);
                cout << "Hello World!" << " " << 3.14 << " " << -25 << aae::flush;
        }
        catch(const_byte_ptr exception)
        {
                fprintf(stderr, "%s\n", exception);
        }
        return 0;
}