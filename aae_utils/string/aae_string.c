#include "aae_string.h"
#include "ctype/aae_ctype.h"
#include "math/aae_math.h"
#include "misc/aae_defines.h"


/** memcpy implementation **/
byte_ptr aae_memcpy(byte_ptr dst, const_byte_ptr src, aae_size_t n)
{
	byte_ptr d_ptr = dst;
	const_byte_ptr s_ptr = src;
	while (n--) *d_ptr++ = *s_ptr++;
	return dst;
}


/** strcpy implementation **/
byte_ptr aae_strcpy(byte_ptr dst, const_byte_ptr src)
{
	byte_ptr r = dst;
	while (*dst++ = *src++);
	return r;

}


/** strcat implementation **/
byte_ptr aae_strcat(byte_ptr dst, const_byte_ptr src)
{
	byte_ptr r = dst;
	while (*dst) ++dst;
	while (*dst++ = *src++);
	return r;
}


/** strlen implementation **/
aae_size_t aae_strlen(const_byte_ptr s)
{

	const_byte_ptr sc;
	for (sc = s; *sc; ++sc);
	return sc - s;

}


/** strcmp implementation **/
i32 aae_strcmp(const_byte_ptr a, const_byte_ptr b)
{
	while (*a)
	{
		if (*a == *b)
		{
			++a;
			++b;
		}
		else
		{
			return *a - *b;
		}
	}
	return *b ? -1 : 0;
}


/** atol implementation **/
i64 aae_atol(const_byte_ptr s)
{
	i64 value = 0;
	i64 sign = 1;
	const_byte_ptr p = s;
	while (s && (aae_isdigit(*p) || (p == s && *p == '-')))
	{
		if (p == s && *p == '-')
		{
			sign = -1;
		}
		else
		{
			value *= 10;
			value += (i64)(*p - '0');
		}
		++p;
	}
	return value * sign;
}


static aae_thread_local byte m_ultoa_buffer[313]; /** will hold the largest base 10 representation of a double **/
static aae_thread_local aae_size_t m_offset = 0ul;
static aae_thread_local const_byte_ptr m_digits = "0123456789abcdef";
/** ultoa implementation **/
byte_ptr aae_ultoa(u64 v, u64 b)
{
	b = aae_min(b, 16);
	byte_ptr p = m_ultoa_buffer + sizeof(m_ultoa_buffer) - m_offset;
	if (!m_offset) *--p = '\0';
	do
	{
		*--p = *(m_digits + v % b);
		v /= b;
	}
	while(v);
	m_offset = 0ul;
	return p;
}


/** ltoa implementation **/
byte_ptr aae_ltoa(i64 v)
{
	byte_ptr p;
	bool is_negative = false;
	if (v < 0)
	{
		is_negative = true;
		v = -v;
	}
	p = aae_ultoa((u64)(v));
	if (is_negative) *--p = '-';
	return p;
}



/**dtoa implementation **/
byte_ptr aae_dtoa(r64 v, u64 prc)
{
	byte_ptr p;
	bool is_negative = false;
	if (v < 0)
	{
		is_negative = true;
		v = -v;
	}
	prc = aae_min(prc, 16);
	u64 p10 = aae_pow10(prc);
	u64 dp = (u64(v * p10)) % p10;
	p = aae_ultoa(dp);
	aae_size_t dp_s = prc - aae_strlen(p);
	while (dp_s-- > 0) /** fill with 0's when necessary **/
		*--p = '0';
	*--p = '.';
	m_offset = prc + 2;
	p = aae_ultoa((u64)v);
	if (is_negative) *--p = '-';
	return p;
}


/** vsprintf implementation **/
void aae_vsprintf(byte_ptr buf, const_byte_ptr fmt, va_list args)
{

	byte_ptr t_string;
	i32 t_pr;
	i64 t_i64;
	u64 t_u64;
	r64 t_r64;
	byte ch;

	const_byte_ptr p = fmt;
	while (ch = *fmt++)
	{
		if (ch == '%')
		{
			t_pr = 0;
			t_string = AAE_NULL;
			while(*fmt && aae_isdigit(*fmt))
			{
				t_pr *= 10;
				t_pr += ((i32)(*fmt - '0'));
				++fmt;
			}
			switch(ch = *fmt++)
			{
				case 'i':
				{
					t_i64 = (i32)va_arg(args, i32);
					t_string = aae_ltoa(t_i64);
					break;
				}
				case 'I':
				{
					t_i64 = (i64)va_arg(args, i64);
					t_string = aae_ltoa(t_i64);
					break;
				}
				case 'u':
				{
					t_u64 = (u32)va_arg(args, u32);
					t_string = aae_ultoa(t_u64);
					break;
				}
				case 'U':
				{
					t_u64 = (u64)va_arg(args, u64);
					t_string = aae_ultoa(t_u64);
					break;
				}
				case 'f':
				case 'F':
				{
					t_pr = !t_pr ? 3 : t_pr;
					t_r64 = (r64)va_arg(args, r64);
					t_string = aae_dtoa(t_r64, t_pr);
					break;
				}
				case 'c':
				{
					m_ultoa_buffer[0] = (byte)va_arg(args, i32);
					m_ultoa_buffer[1] = '\0';
					t_string = m_ultoa_buffer;
					break;
				}
				case 's':
				{
					t_string = (byte_ptr)va_arg(args, byte_ptr);
					break;
				}
				case 'x':
				case 'X':
				{
					t_u64 = (u64)va_arg(args, u64);
					t_string = aae_ultoa(t_u64, 16);
					break;
				}
				case 'b':
				case 'B':
				{
					t_u64 = (u64)va_arg(args, u64);
					t_string = aae_ultoa(t_u64, 2);
					break;
				}
				case 'o':
				case 'O':
				{
					t_u64 = (u64)va_arg(args, u64);
					t_string = aae_ultoa(t_u64, 8);
					break;
				}
			}
			if (t_string)
			{
				aae_strcpy(buf, t_string);
				buf += aae_strlen(t_string);
			}
		}
		else
		{
			*buf++ = ch;
		}
	}
	*buf = '\0';
}


/** sprintf implementation **/
void aae_sprintf(byte_ptr buf, const_byte_ptr fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	aae_vsprintf(buf, fmt, args);
	va_end(args);
}