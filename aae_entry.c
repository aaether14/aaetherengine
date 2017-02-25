extern "C" void exit(int);
extern int aae_main(void);


extern "C" void _start() 
{
        #if defined(AAE_LINUX_PLATFORM) && defined(AAE_64BIT_BUILD)
        __asm__ __volatile__("subq $8, %rsp");
        #endif
       exit(aae_main());
}