/*
 *  malloc-test
 *  This code is based on the one at: http://www.citi.umich.edu/projects/linux-scalability/reports/malloc.html
 *  Syntax:
 *  malloc-test [ size [ iterations [ thread count ]]]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "aae_lib_allocator.h"


#define USE_AAE 1
#if USE_AAE == 1
#define malloc aae_malloc
#define free aae_free
#endif


static unsigned size = 512;
static unsigned iteration_count = 10000000;
static unsigned thread_count = 50;


void * dummy(unsigned i)
{
    return NULL;
}


void* run_test(void*)
{
    unsigned int i;
    unsigned request_size = size;
    unsigned total_iterations = iteration_count;
    struct timeval start, end, null, elapsed, adjusted;

    /*
     * Time a null loop.  We'll subtract this from the final
     * malloc loop results to get a more accurate value.
     */
    gettimeofday(&start, NULL);
    for (i = 0; i < total_iterations; i++)
    {
        void* buf;
        buf = dummy(i);
        buf = dummy(i);
    }
    gettimeofday(&end, NULL);

    null.tv_sec = end.tv_sec - start.tv_sec;
    null.tv_usec = end.tv_usec - start.tv_usec;
    if (null.tv_usec < 0) {
        null.tv_sec--;
        null.tv_usec += 1000000;
    }

    /*
     * Run the real malloc test
     */
    gettimeofday(&start, NULL);
    for (i = 0; i < total_iterations; i++) {
        void * buf;
        buf = malloc(request_size);
        free(buf);
    }
    gettimeofday(&end, NULL);

    elapsed.tv_sec = end.tv_sec - start.tv_sec;
    elapsed.tv_usec = end.tv_usec - start.tv_usec;
    if (elapsed.tv_usec < 0) {
        elapsed.tv_sec--;
        elapsed.tv_usec += 1000000;
    }

    /*
     * Adjust elapsed time by null loop time
     */
    adjusted.tv_sec = elapsed.tv_sec - null.tv_sec;
    adjusted.tv_usec = elapsed.tv_usec - null.tv_usec;
    if (adjusted.tv_usec < 0) {
        adjusted.tv_sec--;
        adjusted.tv_usec += 1000000;
    }
    fprintf(stderr, "Thread %lo adjusted timing: %ld.%06ld seconds for %d requests"
        " of %d bytes.\n", pthread_self(),
        adjusted.tv_sec, adjusted.tv_usec,
        total_iterations,
        (int) request_size);

    return NULL;
}



int main(int argc, char *argv[])
{

    unsigned i;
    pthread_t thread[thread_count];

    /*
     * Invoke the tests
     */
    fprintf(stderr, "Starting test...\n");
    for (i=1; i<= thread_count; i++)
        if (pthread_create(&(thread[i]), NULL, &run_test, NULL))
            fprintf(stderr, "failed.\n");

    /*
     * Wait for tests to finish
     */
    for (i=1; i<=thread_count; i++)
        pthread_join(thread[i], NULL);

    exit(0);
}
