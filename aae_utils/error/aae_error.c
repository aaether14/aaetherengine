#include "aae_error.h"


#ifdef AAE_MULTITHREADED
#include <pthread.h>
#include "aae_lib_allocator.h"
#include "../os/linux/aae_os_functions.h"
static bool was_init = false;
static pthread_key_t thread_specific_error_memory_key;
static pthread_mutex_t thread_specific_error_memory_mutex;
static void thread_specific_error_memory_close_handler(void * thread_spcific_error_memory) 
{
	pthread_mutex_lock(&thread_specific_error_memory_mutex);
	aae_free(thread_spcific_error_memory);
	pthread_mutex_unlock(&thread_specific_error_memory_mutex);
}
static void init_thread_specific_error_memory()
{ 
	if (pthread_key_create(&thread_specific_error_memory_key, &thread_specific_error_memory_close_handler))
	{
		aae_write(stderr, "Could not create thread specific error key!\n", 44);
		aae_exit(1);
	}
	if (pthread_mutex_init(&thread_specific_error_memory_mutex, NULL))
	{
		aae_write(stderr, "Could not create errno mutex!\n", 30);
		aae_exit(1);
	}
	pthread_mutex_lock(&thread_specific_error_memory_mutex);
	was_init = true;
	pthread_mutex_unlock(&thread_specific_error_memory_mutex);
}
static void manage_thread_specific_error_memory()
{
	if (!was_init)
		init_thread_specific_error_memory();
	if (pthread_getspecific(thread_specific_error_memory_key) == NULL)
	{
		pthread_mutex_lock(&thread_specific_error_memory_mutex);
		i32 * thread_spcific_error_memory = (i32*)aae_malloc(sizeof(i32));
		pthread_mutex_unlock(&thread_specific_error_memory_mutex);
		*thread_spcific_error_memory = 0; /** no error at first **/
		pthread_setspecific(thread_specific_error_memory_key, thread_spcific_error_memory);
	}
}
i32 aae_get_error()
{
	manage_thread_specific_error_memory();
	i32* thread_spcific_error_memory = (i32*)pthread_getspecific(thread_specific_error_memory_key);
	if (thread_spcific_error_memory)
		return *thread_spcific_error_memory;
	else
		return AAE_COULD_NOT_GET_THREAD_SPECIFIC_MEMORY;
}
void aae_set_error(i32 error_code)
{
	manage_thread_specific_error_memory();
	i32* thread_spcific_error_memory = (i32*)pthread_getspecific(thread_specific_error_memory_key);
	if (thread_spcific_error_memory)
		*thread_spcific_error_memory = error_code;
}
#else
static i32 last_error_code = 0; /** no error at first **/
i32 aae_get_error()
{
	return last_error_code;
}
void aae_set_error(i32 error_code)
{
	last_error_code = error_code;
}
#endif