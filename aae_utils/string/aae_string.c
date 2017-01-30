#include "aae_string.h"
#include "math/aae_math.h"
#include "aae_defines.h"


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
	while (s && ((*p >= '0' && *p <= '9') || (p == s && *p == '-')))
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
/** ultoa implementation **/
byte_ptr aae_ultoa(u64 v, aae_size_t o)
{
	byte_ptr p = m_ultoa_buffer + sizeof(m_ultoa_buffer) - o;
	if (!o) *--p = '\0';
	do
	{
		*--p = '0' + v % 10;
		v /= 10;
	}
	while(v);
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
		v = - v;
	}
	prc = aae_min(prc, 16);
	u64 p10 = aae_pow10(prc);
	u64 dp = (u64(v * p10)) % p10;
	p = aae_ultoa(dp);
	*--p = '.';
	p = aae_ultoa(((u64)v), prc + 2);
	if (is_negative) *--p = '-';
	return p;
}


/** vsprintf implementation **/
void aae_vsprintf(byte_ptr buf, const_byte_ptr fmt, va_list args)
{

	byte_ptr t_string;
	i64 t_i64;
	u64 t_u64;
	r64 t_r64;
	byte ch;

	const_byte_ptr p = fmt;
	while (ch = *fmt++)
	{
		if (ch == '%')
		{
			t_string = AAE_NULL;
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
					t_i64 = (i32)va_arg(args, i64);
					t_string = aae_ltoa(t_i64);
					break;
				}
				case 'u':
				{
					t_u64 = (u32)va_arg(args, u32);
					t_string = aae_ultoa(t_u64);
					break;
				}
				case 'f':
				{
					t_r64 = (r64)va_arg(args, r64);
					t_string = aae_dtoa(t_r64, 3);
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
}


/** sprintf implementation **/
void aae_sprintf(byte_ptr buf, const_byte_ptr fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	aae_vsprintf(buf, fmt, args);
	va_end(args);
}