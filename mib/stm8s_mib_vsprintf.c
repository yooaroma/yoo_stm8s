//
// vsprintf.c
//
// Print formatting routines
//
// Copyright (C) 2002 Michael Ringgaard. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.  
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.  
// 3. Neither the name of the project nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
// SUCH DAMAGE.
// 

//------------------------------------------------------------------------------
//
//  Extern:  OEMWriteDebugString
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "stm8s.h"
 
#define MMRINT   int  // int // return
#define MMAINT   int // s32  // int
#define MMAUINT  unsigned int // u32  // unsigned int
#define MMINT    s32 // int // s32  // int
#define MMUINT   u32 // unsigned int // u32  // unsigned int
#define MMULONG  u32 // unsigned long // u32  // unsigned long
#define MMLONG   s32 // long // s32  // long
#define MMUSIGN  u16 // unsigned // u16  // unsigned  
#define MMUCHAR  u8 // unsigned char // u8   // unsigned char 
#define MMUSHORT u16   // unsigned short 

#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'

#define is_digit(c) ((c) >= '0' && (c) <= '9')

static char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
static size_t strnlen(const char *s, size_t count)
{
	const char *sc;
	for (sc = s; *sc != '\0' && count--; ++sc);
	return sc - s;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
static MMINT skip_atoi(const char **s)
{
	MMINT i = 0;
	while (is_digit(**s)) i = i*10 + *((*s)++) - '0';
	return i;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
static char *number(char *str, MMLONG num, MMINT base, MMINT size, MMINT precision, MMINT type)
{
	char c, sign, tmp[66];
	char *dig = digits;
	MMINT i;

	if (type & LARGE)  dig = upper_digits;
	if (type & LEFT) type &= ~ZEROPAD;
	if (base < 2 || base > 36) return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN)
	{
		if (num < 0)
		{
			sign = '-';
			num = -num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}

	if (type & SPECIAL)
	{
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}

	i = 0;

	if (num == 0)
		tmp[i++] = '0';
	else
	{
		while (num != 0)
		{
			tmp[i++] = dig[((MMULONG) num) % (MMUSIGN) base];
			num = ((MMULONG) num) / (MMUSIGN) base;
		}
	}

	if (i > precision) precision = i;
	size -= precision;
	if (!(type & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
	if (sign) *str++ = sign;

	if (type & SPECIAL)
	{
		if (base == 8)
			*str++ = '0';
		else if (base == 16)
		{
			*str++ = '0';
			*str++ = digits[33];
		}
	}

	if (!(type & LEFT)) while (size-- > 0) *str++ = c;
	while (i < precision--) *str++ = '0';
	while (i-- > 0) *str++ = tmp[i];
	while (size-- > 0) *str++ = ' ';

	return str;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

MMRINT mm_vsprintf(char *buf, const char *fmt, va_list args)
{
	MMINT len;
	MMULONG num;
	MMINT i, base;
	char *str;
	char *s;

	MMINT flags;            // Flags to number()

	MMINT field_width;      // Width of output field
	MMINT precision;        // Min. # of digits for integers; max number of chars for from string
	MMINT qualifier;        // 'h', 'l', or 'L' for integer fields
	
	char vStr[] = "<NULL>";

	for (str = buf; *fmt; fmt++)
	{
		if (*fmt != '%')
		{
			*str++ = *fmt;
			continue;
		}

		// Process flags
		flags = 0;
repeat:
		fmt++; // This also skips first '%'
		switch (*fmt)
		{
		case '-': flags |= LEFT; goto repeat;
		case '+': flags |= PLUS; goto repeat;
		case ' ': flags |= SPACE; goto repeat;
		case '#': flags |= SPECIAL; goto repeat;
		case '0': flags |= ZEROPAD; goto repeat;
		}

		// Get field width
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*')
		{
			fmt++;
			field_width = va_arg(args, MMINT);
			if (field_width < 0)
			{
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// Get the precision
		precision = -1;
		if (*fmt == '.')
		{
			++fmt;    
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*')
			{
				++fmt;
				precision = va_arg(args, MMINT);
			}
			if (precision < 0) precision = 0;
		}

		// Get the conversion qualifier
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
		{
			qualifier = *fmt;
			fmt++;
		}

		// Default base
		base = 10;

		switch (*fmt)
		{
		case 'c':
			if (!(flags & LEFT)) while (--field_width > 0) *str++ = ' ';
			*str++ = (MMUCHAR) va_arg(args, MMAINT); // test ok...
			while (--field_width > 0) *str++ = ' ';
			continue;

		case 's':
			s = va_arg(args, char *);
			if (!s) s = vStr; // "<NULL>";
			len = strnlen(s, precision);
			if (!(flags & LEFT)) while (len < field_width--) *str++ = ' ';
			for (i = 0; i < len; ++i) *str++ = *s++;
			while (len < field_width--) *str++ = ' ';
			continue;

		case 'p':
			if (field_width == -1)
			{
				field_width = 2 * sizeof(void *);
				flags |= ZEROPAD;
			}
			str = number(str, (MMULONG) va_arg(args, void *), 16, field_width, precision, flags);
			continue;

		case 'n':
			if (qualifier == 'l')
			{
				MMLONG *ip = va_arg(args, MMLONG *);
				*ip = (str - buf);
			}
			else
			{
				MMINT *ip = va_arg(args, MMINT *);
				*ip = (str - buf);
			}
			continue;
			
			// Integer number formats - set up the flags and "break"
		case 'o':
			base = 8;
			break;

		case 'X':
			flags |= LARGE;

		case 'x':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;

		case 'u':
			break;
			
		default:
			if (*fmt != '%') *str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			continue;
		}

		if (qualifier == 'l')
			num = va_arg(args, MMUINT); // MMULONG); // test ok..
		#if 0
		else if (qualifier == 'h')
		{
			if (flags & SIGN)
				num = va_arg(args, short);
			else
				num = va_arg(args, MMUSHORT);
		}
		#endif
		else if (flags & SIGN)
			num = va_arg(args, MMAINT); // test ok...
		else
			num = va_arg(args, MMAUINT);

		str = number(str, num, base, field_width, precision, flags);
	}

	*str = '\0';
	return str - buf;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
MMRINT mib_vsprintf(char *vpBuffer,char *format, ...)
{
	va_list ap;
	MMRINT rc;
	{
		va_start(ap, format);
		rc = mm_vsprintf(vpBuffer, format, ap);
		va_end(ap);
	}
	return (rc);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
extern void MibWriteDebugByte(uint8_t c);
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/*
 * write formatted output to stdout immediately 
*/
MMRINT mib_printf(char *format, ...)
{
		va_list ap;
		char buffer[128];
		char *v_pString;
		MMRINT rc;

		va_start(ap, format);
		rc = mm_vsprintf(buffer, format, ap);
		va_end(ap);

		v_pString = (char *)buffer;
		while (*v_pString != 0)	MibWriteDebugByte((uint8_t)*v_pString++);
		return (rc);
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
