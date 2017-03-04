#ifndef AAE_FILESTREAM_H
#define AAE_FILESTREAM_H
#include "aae_ringbuffer.h"
#if defined(AAE_LINUX_PLATFORM)
#include <fcntl.h>
#include <unistd.h>
#define AAE_READ_ONLY O_RDONLY
#define AAE_WRITE_ONLY O_WRONLY | O_TRUNC
#define AAE_READ_WRITE O_RDWR
#elif defined(AAE_WINDOWS_PLATOFRM)
#include <windows.h>
#define AAE_READ_ONLY OF_READ
#define AAE_WRITE_ONLY OF_WRITE | OF_CREATE
#define AAE_READ_WRITE OF_READWRITE
#endif
#ifndef AAE_STRING_H
byte_ptr aae_memcpy(byte_ptr, const_byte_ptr, aae_size_t);
aae_size_t aae_strlen(const_byte_ptr);
byte_ptr aae_ltoa(i64);
byte_ptr aae_ultoa(u64, u64);
byte_ptr aae_dtoa(r64, u64);
#endif


namespace aae
{


template <aae_size_t BUFFER_SIZE = 512ul>
class filestream : public ringbuffer<byte>
{
	#if defined(AAE_LINUX_PLATFORM)
	typedef int open_mode_t;
	typedef int file_handle_t;
	#elif defined(AAE_WINDOWS_PLATOFRM)
	typedef UINT open_mode_t;
	typedef HFILE file_handle_t;
	#endif
	file_handle_t m_handle;
	open_mode_t m_open_mode;
	u64 m_float_precision = 3;
	bool m_has_opened_file;
	public:
	filestream(const_byte_ptr filename, open_mode_t mode) : 
		ringbuffer<byte>(BUFFER_SIZE, aae_mallocator), 
		m_open_mode(mode),
		m_has_opened_file(true)
	{
		#if defined(AAE_LINUX_PLATFORM)
		if ((m_handle = open(filename, mode)) == -1) throw("Could not open file!");
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	filestream(file_handle_t handle, open_mode_t mode) : 
		ringbuffer<byte>(BUFFER_SIZE, aae_mallocator),
		m_handle(handle), 
		m_open_mode(mode), 
		m_has_opened_file(false) { }
	virtual ~filestream()
	{
		flush();
		#if defined(AAE_LINUX_PLATFORM)
		if (m_has_opened_file) close(m_handle);
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	virtual bool push(const byte& c_data)
	{
		if (m_open_mode == AAE_READ_ONLY) return false;
		return ringbuffer<byte>::push(c_data);
	}
	virtual bool pop(byte& c_data)
	{
		if (m_open_mode == AAE_WRITE_ONLY) return false;
		return ringbuffer<byte>::pop(c_data);
	}
	inline bool flush() { if (m_open_mode == AAE_READ_ONLY) return false; return overflow(); }
	inline void set_float_precision(const u64& precision) { m_float_precision = precision; }
	inline const u64& get_float_precision() const { return m_float_precision; }
	protected:
	virtual bool underflow()
	{
		#if defined(AAE_LINUX_PLATFORM)
		i32&& c_head = read(m_handle, get_buffer(), get_size() - 1);
		if (c_head == -1) throw("Could not read in underflow()");
		else if(!c_head) return false;
		set_head(c_head);
		set_tail(0);
		return true;
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	virtual bool overflow()
	{
		#if defined(AAE_LINUX_PLATFORM)
		if (get_head() < get_tail())
		{
			aae_size_t&& flush_size = get_size() + get_head() - get_tail();
			aae_size_t&& first_half = get_size() - get_tail();
			byte_ptr temp = (byte_ptr)aae_mallocator->Allocate(flush_size);
			if (!temp) throw("Could not allocate temp buffer in overflow()");
			aae_memcpy(temp, get_buffer() + get_tail(), first_half);
			aae_memcpy(temp + first_half, get_buffer(), get_head());
			if (write(m_handle, temp, flush_size) == -1) throw("Could not write in overflow()");
			aae_mallocator->Deallocate(temp);
		}
		else
		{
			if (write(m_handle, get_buffer() + get_tail(), get_head() - get_tail()) == -1) throw("Could not write in overflow()");
		}
		set_head(0);
		set_tail(0);
		return true;
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
};


class stream_flush {} flush;
class float_precision 
{
	u64 m_precision;
	public:
	float_precision() : m_precision(3) {}
	float_precision(const u64& precision) { m_precision = precision; } 
	const u64& get_precision() const { return m_precision; }
};


template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, stream_flush) { stream.flush(); return stream; }
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, float_precision precision)
{ 
 	stream.set_float_precision(precision.get_precision()); 
 	return stream; 
}
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, byte character) { stream.push(character); return stream; }
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, const_byte_ptr string) 
{
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, i64 integer)
{	
	byte_ptr string = aae_ltoa(integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, i32 integer)
{	
	byte_ptr string = aae_ltoa(integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}  
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, u64 unsigned_integer)
{	
	byte_ptr string = aae_ultoa(unsigned_integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, u32 unsigned_integer)
{	
	byte_ptr string = aae_ultoa(unsigned_integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template <aae_size_t BUFFER_SIZE>
inline filestream<BUFFER_SIZE>& operator<<(filestream<BUFFER_SIZE>& stream, r64 real_value)
{	
	byte_ptr string = aae_dtoa(real_value, stream.get_float_precision());
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}



}
#endif