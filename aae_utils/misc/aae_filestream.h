#ifndef AAE_FILESTREAM_H
#define AAE_FILESTREAM_H
#include "aae_ringbuffer.h"
#include "misc/aae_type_traits.h"
#if defined(AAE_LINUX_PLATFORM)
#include <fcntl.h>
#include <unistd.h>
#elif defined(AAE_WINDOWS_PLATOFRM)
#include <windows.h>
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


class filesource
{

	#if defined(AAE_LINUX_PLATFORM)
	using open_mode_t = int;
	using file_handle_t = int;
	#elif defined(AAE_WINDOWS_PLATOFRM)
	using open_mode_t = UINT;
	using file_handle_t = HFILE;
	#endif
	file_handle_t m_handle;
	open_mode_t m_open_mode;
	bool m_has_opened_file = false;
	bool m_is_error = false;
	filesource(const filesource& other) = default;
	filesource& operator=(const filesource& other) = default;
	filesource& operator=(filesource&& other) = delete;
	public:
	#if defined(AAE_LINUX_PLATFORM)
	static constexpr open_mode_t read_only_mode = O_RDONLY;
	static constexpr open_mode_t write_only_mode = O_WRONLY | O_TRUNC;
	#elif defined(AAE_WINDOWS_PLATOFRM)
	static constexpr open_mode_t read_only_mode = OF_READ;
	static constexpr open_mode_t write_only_mode = O_WRITE | OF_CREATE;
	#endif
	filesource(filesource&& other)
	{
		*this = other;
		other.m_has_opened_file = false;
	}
	filesource(const_byte_ptr filename, open_mode_t mode) : m_open_mode(mode)
	{
		#if defined(AAE_LINUX_PLATFORM)
		if ((m_handle = open(filename, mode)) == -1) { m_is_error = true; throw("Could not open file!"); }
		m_has_opened_file = true;
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	filesource(file_handle_t handle, open_mode_t mode) :
		m_handle(handle),
		m_open_mode(mode) {}
	~filesource()
	{
		#if defined(AAE_LINUX_PLATFORM)
		if (m_has_opened_file) if (close(m_handle) == -1) { m_is_error = true; throw("Error while closing file!"); }
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	inline const bool& error() const { return m_is_error; }
	inline bool can_read() const { return m_open_mode == read_only_mode; }
	inline bool can_write() const { return m_open_mode == write_only_mode; }
	inline aae_size_t write(const_byte_ptr buffer, const aae_size_t& write_size)
	{
		#if defined(AAE_LINUX_PLATFORM)
		aae_ssize_t&& result = ::write(m_handle, buffer, write_size);
		if (result == -1) { m_is_error = true; throw("Could not write to file!"); }
		return result;
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}
	inline aae_size_t read(byte_ptr buffer, const aae_size_t& read_size)
	{
		#if defined(AAE_LINUX_PLATFORM)
		aae_ssize_t&& result = ::read(m_handle, buffer, read_size);
		if (result == -1) { m_is_error = true; throw("Could not read from file!"); }
		return result;
		#elif defined(AAE_WINDOWS_PLATOFRM)
		#endif
	}

};


class stream_flush {} flush;
class float_precision 
{
	u64 m_precision;
	public:
	explicit float_precision(const u64& precision) { m_precision = precision; } 
	const u64& get() const { return m_precision; }
};


template<typename SourcePolicy>
class filestream : public ringbuffer<byte>
{
	SourcePolicy m_source_policy;
	float_precision m_float_precision = float_precision(3);
	public:
	filestream(SourcePolicy&& source_policy) : ringbuffer<byte>(511, aae::mallocator), m_source_policy(aae::move(source_policy)) {}
	virtual ~filestream()
	{
		if (!m_source_policy.error()) flush();
	}
	virtual bool push(const byte& c_data)
	{
		if (!m_source_policy.can_write()) return false;
		return ringbuffer<byte>::push(c_data);
	}
	virtual bool pop(byte& c_data)
	{
		if (!m_source_policy.can_read()) return false;
		return ringbuffer<byte>::pop(c_data);
	}
	inline bool flush() 
	{ 
		if (!m_source_policy.can_write()) return false; 
		return overflow(); 
	}
	inline float_precision& precision() { return m_float_precision; } 
	protected:
	virtual bool underflow()
	{
		aae_size_t&& result = m_source_policy.read(get_buffer(), get_size() - 1);
		set_head(result);
		set_tail(0);
		return true;
	}
	virtual bool overflow()
	{
		if (get_head() < get_tail())
		{
			aae_size_t&& flush_size = get_size() + get_head() - get_tail();
			aae_size_t&& first_half = get_size() - get_tail();
			byte_ptr temp = (byte_ptr)aae::mallocator.Allocate(flush_size);
			aae_memcpy(temp, get_buffer() + get_tail(), first_half);
			aae_memcpy(temp + first_half, get_buffer(), get_head());
			m_source_policy.write(temp, flush_size);
			aae::mallocator.Deallocate(temp);
		}
		else
		{
			m_source_policy.write(get_buffer() + get_tail(), get_head() - get_tail());
		}
		set_head(0);
		set_tail(0);
		return true;
	}
};


template <typename FileStream>
inline FileStream& operator<<(FileStream& lhs, const stream_flush&) 
{ 
	lhs.flush(); 
	return lhs; 
}
template <typename FileStream>
inline FileStream& operator<<(FileStream& lhs, const float_precision& rhs)
{ 
 	lhs.precision() = rhs;
 	return lhs; 
}
template <typename FileStream>
inline FileStream& operator<<(FileStream& lhs, const byte& rhs) 
{ 
	lhs.push(rhs); 
	return lhs; 
}
template <typename FileStream>
inline FileStream& operator<<(FileStream& lhs, const_byte_ptr rhs) 
{
	aae_size_t rhs_size = aae_strlen(rhs);
	for (aae_size_t it = 0; it < rhs_size; ++it)
		lhs.push(rhs[it]); 
	return lhs;
}
template<typename FileStream, typename T, typename enable_if<is_signed<T>::value, i32>::type = 0>
inline FileStream& operator<<(FileStream& stream, const T& integer)
{	
	byte_ptr string = aae_ltoa(integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template<typename FileStream, typename T, typename enable_if<is_unsigned<T>::value, i32>::type = 0>
inline FileStream& operator<<(FileStream& stream, const T& unsigned_integer)
{	
	byte_ptr string = aae_ultoa(unsigned_integer);
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}
template <typename FileStream, typename T, typename enable_if<is_real<T>::value, i32>::type = 0>
inline FileStream& operator<<(FileStream& stream, const T& real_value)
{	
	byte_ptr string = aae_dtoa(real_value, stream.precision().get());
	for (aae_size_t it = 0; it < aae_strlen(string); ++it)
		stream.push(string[it]); 
	return stream;
}



}
#endif